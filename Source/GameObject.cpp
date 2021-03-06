#include "GameObject.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include "MathGeoLib\Algorithm\Random\LCG.h"
#include <cmath>
#include "ComponentDefs.h"
#include "JSON.h"

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(bool _active)
{
	active = _active;

	if (!child_list.empty())
	{
		for (int i = 0; i < child_list.size(); i++)
		{
			child_list[i]->SetActive(_active); 
		}
	}
}

bool GameObject::IsStatic() const
{
	return is_static;
}

void GameObject::SetStatic(bool _static)
{
	is_static = _static;

	if (is_static)
		App->scene_intro->AddToStaticRecursive(this);
	else
		App->scene_intro->DeleteFromStaticRecursive(this); 
}

void GameObject::SetName(const char * _name)
{
	name = _name; 
}

const char * GameObject::GetName()const
{
	return name.c_str();
}

void GameObject::SetID(uint _id)
{
	unique_id = _id;
}

uint GameObject::GetID()const
{
	return unique_id;
}

GameObject::GameObject()
{
	math::LCG random;
	unique_id = random.Int();

	parent = nullptr;
	active = true; 
	is_static = false; 
	is_root = false; 

	ComponentTransform* trans = new ComponentTransform(this);
	PushComponent(trans);

	transform = trans; 

	trans->SetIdentityTransform();
	trans->type = COMPONENT_TRANSFORM;

	LOG("GameObject created with ID: %d", unique_id); 
}

void GameObject::Start()
{
		LOG("Game Object Created");
		SetName("Root");
	
}

bool GameObject::Save(json_file & parent_config) const
{
	json_file my_config;

	// This is only useful when we are duplicating already existing gameobjects
	uint uid_to_save = unique_id;
	uint parent_uid = (parent) ? parent->GetID() : 0;

	//Save IDs
	my_config.SetUInt("UUID", uid_to_save);
	my_config.SetUInt("ParentUUID", parent_uid);
	
	//Save Name
	my_config.SetString("Name", name.c_str());

	// Save Components
	my_config.SetArray("Components");
	for (vector<Component*>::const_iterator it = component_list.begin(); it != component_list.end(); ++it)
	{
		json_file component;
		component.SetInt("Type", (*it)->type);
		(*it)->OnSave(component);
		my_config.SetNodeEntry(component);
	}
	parent_config.SetNodeEntry(my_config);
	// Recursively Children Call
	for (vector<GameObject*>::const_iterator it = child_list.begin(); it != child_list.end(); ++it)
	{
		(*it)->Save(parent_config);
	}

	return true;
}

void GameObject::Load(json_file * config)
{
	// UID
	unique_id = config->GetUInt("UID", unique_id);
	uint parent = config->GetUInt("ParentUID", 0);

	// Name
	name = config->GetString("Name", "Unnamed");

	// Now Load all my components
	int count = config->GetArraySize("Components");

	for (int i = 0; i < count; ++i)
	{
		json_file component_conf(config->GetArray("Components", i));
		component_type type = (component_type)component_conf.GetInt("Type", component_type::COMPONENT_NULL);
		if (type != component_type::COMPONENT_NULL)
		{
			Component* component = AddEmptyComponent(type);
			component->OnLoad(&component_conf);
		}
		else
			LOG("Cannot load component type UNKNOWN for gameobject %s", name.c_str());
	}
}

void GameObject::Draw()
{
	App->performance.gameobject_perf_timer.Start(); 

	if (!active)
		return; 

	if (!component_list.empty())
	{
		for (int i = 0; i < component_list.size(); i++)
		{
			if (component_list[i]->IsActive() == false)
				return;

			component_list[i]->Update();
		}
	}	

	if (!child_list.empty())
	{
		for (int i = 0; i < child_list.size(); i++)
		{
			child_list[i]->Draw();
		}
	}

	App->performance.SaveRunTimeData("gameobject"); 
}
void GameObject::Delete()
{

	if (parent != nullptr)
		DeleteParent();

	DeleteComponents();

}

bool GameObject::IsChild(const GameObject* go) const
{
	for (vector<GameObject*>::const_iterator it = go->child_list.begin(); it != go->child_list.end(); ++it)
	{
		if (this == *it || IsChild(*it) == true)
			return true;
	}

	return false;
}

void GameObject::RecursiveAdaptBoundingBox(float4x4 transform, GameObject* go_to_adapt)
{
	AdaptBoundingBox(go_to_adapt);

	if (!go_to_adapt->child_list.empty())
	{
		for (int i = 0; i < go_to_adapt->child_list.size(); i++)
		{
			ComponentMeshRenderer* child_mr = (ComponentMeshRenderer*)go_to_adapt->child_list[i]->GetComponent(COMPONENT_MESH_RENDERER);
			ComponentTransform* child_trans = (ComponentTransform*)go_to_adapt->child_list[i]->GetComponent(COMPONENT_TRANSFORM);

			if (child_mr != nullptr)
			{
				RecursiveAdaptBoundingBox(child_trans->GetGlobalTransform(), child_list[i]);
				child_trans->SetModified(false); 
			}
				
		}
	}
}

void GameObject::AdaptBoundingBox(GameObject* go_to_adapt)
{
	ComponentMeshRenderer* mr = (ComponentMeshRenderer*)go_to_adapt->GetComponent(COMPONENT_MESH_RENDERER);
	ComponentParticleEmmiter* emiter = (ComponentParticleEmmiter*)go_to_adapt->GetComponent(COMPONENT_PARTICLE_EMMITER); 
	ComponentTransform* trans = (ComponentTransform*)go_to_adapt->GetComponent(COMPONENT_TRANSFORM);

	if (mr != nullptr)
	{
		mr->bounding_box.SetNegativeInfinity();
		mr->bounding_box.Enclose(mr->vertices, mr->num_vertices);
		mr->bounding_box.TransformAsAABB(trans->GetGlobalTransform());
	}

	if (emiter != nullptr)
	{
		emiter->emit_area->bounding_box.SetNegativeInfinity();
		emiter->emit_area->bounding_box.Enclose(emiter->emit_area->vertices, emiter->emit_area->num_vertices);
		emiter->emit_area->bounding_box.TransformAsAABB(trans->GetGlobalTransform());
	}
}

Component * GameObject::GetComponent(component_type new_component_type, int skip_num)
{

	for (vector<Component*>::iterator it = component_list.begin();it != component_list.end(); it++)
	{
		if ((*it)->type == new_component_type)		
			return (*it); 
	}

	return nullptr; 
	
}

Component * GameObject::GetComponent(int index)
{
	int count = 0; 
	for (std::vector<Component*>::iterator it = component_list.begin(); it != component_list.end(); it++)
	{
		if (count++ == index)
			return (*it); 
	}
}

AABB GameObject::GetBoundingBox()
{
	AABB ret; 

	ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponent(COMPONENT_MESH_RENDERER); 

	if (mr != nullptr)
	{
		ret = mr->bounding_box; 
	}

	return ret;
}

GameObject * GameObject::GetChild(uint id)const
{
	return child_list.at(id);
}

bool GameObject::IsCompEmpty()
{
	return component_list.empty();
}

bool GameObject::IsChildEmpty()
{
	return child_list.empty();
}

float GameObject::DistanceTo(float3 point)
{
	ComponentTransform* trans = (ComponentTransform*)GetComponent(COMPONENT_TRANSFORM);
	float3 dir = trans->GetGlobalPosition() - point;
	return dir.Length();
}

bool GameObject::IsRoot() const
{
	return is_root;;
}

void GameObject::SetRoot(const bool & _root)
{
	is_root = _root; 
}

bool GameObject::IsLooking4Parent()
{
	return looking_for_parent;
}

void GameObject::SetLooking4Parent(bool is_searching)
{
	looking_for_parent = is_searching;
}

bool GameObject::IsLooking4Reference()
{
	return looking_for_reference;
}

void GameObject::SetLooking4Reference(bool is_searching)
{
	looking_for_reference = is_searching; 
}

void GameObject::ShowBB(bool show_it)
{
	ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponent(COMPONENT_MESH_RENDERER); 

	if (mr)
		mr->show_bb = show_it; 
		
}

void GameObject::Serialize(json_file * file)
{
	// Set the name
	file->SetString("name", name.c_str());

	file->SetBool("static", is_static);

	file->SetArray("Components");

	// Save components
	for (vector<Component*>::iterator it = component_list.begin(); it != component_list.end(); it++)
	{
		// Add and move to a new section on the components array
		json_file* comp_doc = file->GetNode();
		comp_doc->AddSectionToArray("Components");
		comp_doc->MoveToSectionFromArray("Components",file->GetArraySize("Components") - 1);

		comp_doc->SetInt("type", (*it)->GetComponentType());
		(*it)->Serialize(comp_doc);
	}

}

void GameObject::Serialize(json_file file)
{
	////file.AddSectionToArray("GameObjects");
	////file.MoveToSectionFromArray("GameObjects", file.GetArraySize("GameObjects") - 1);
	//file.SetInt("UID", unique_id);
	//file.SetString("name", name.c_str());
	//file.SetBool("is_active", active);
	//file.SetBool("is_static", is_static);
	///*if (parent != nullptr)
	//	file.SetInt("parent", parent->GetID());
	//else
	//	file.SetInt("parent", 0);*/
	////file.SetInt("new_child_id", new_child_id);


	////file.MoveToRoot();
	////Save components 
	//for (std::vector<Component*>::iterator c = component_list.begin(); c != component_list.end(); ++c)
	//{
	//	json_file* comp_doc = file.GetNode();
	//	file.AddSectionToArray("Components");
	//	file.MoveToSectionFromArray("Components", file.GetArraySize("Components") - 1);

	//	comp_doc.SetInt("type", (*c)->type);
	//	(*c)->Serialize(file);
	//	//file.MoveToRoot();
	//}
	////Save childs
	///*for (std::vector<GameObject*>::iterator go = child_list.begin(); go != child_list.end(); ++go)
	//{
	//	(*go)->Serialize(file);
	//	file.MoveToRoot();
	//}*/
}

void GameObject::SerializeLoad(json_file* file)
{
	name = file->GetString("name");

	bool stat = file->GetBool("static");
	if (stat)
		App->scene_intro->AddToStatic(this);

	int components_count = file->GetArraySize("Components");

	for (int i = 0; i < components_count; i++)
	{
		json_file* comp_node = file->GetNode();
		comp_node->MoveToSectionFromArray("Components", i);

		component_type type = static_cast<component_type>((int)comp_node->GetInt("type", 0));

		if (type != COMPONENT_TRANSFORM)
		{
			Component* c = AddEmptyComponent(type);
			c->SerializeLoad(comp_node);
		}
		else
			transform->SerializeLoad(comp_node);
	}
}

void GameObject::SerializeSave(json_file* file)
{
	// Set the name
	file->SetString("name", name.c_str());

	file->SetBool("static", is_static);

	file->SetArray("Components");

	// Save components
	for (vector<Component*>::iterator it = component_list.begin(); it != component_list.end(); it++)
	{
		// Add and move to a new section on the components array
		json_file* comp_doc = file->GetNode();
		comp_doc->AddSectionToArray("Components");
		comp_doc->MoveToSectionFromArray("Components", file->GetArraySize("Components") - 1);

		comp_doc->SetInt("type", (*it)->type);
		(*it)->SerializeSave(comp_doc);
	}
}


uint GameObject::GetNumChilds()const
{
	return child_list.size();
}

uint GameObject::GetNumComponents()const
{
	return component_list.size();
}

void GameObject::DeleteComponents()
{
	for (vector<Component*>::iterator it = component_list.begin(); it != component_list.end(); it++)
	{
		(*it)->DeleteComponent();
	}

	component_list.clear();
}

GameObject * GameObject::GetParent()const
{
	return parent;
}

void GameObject::SetParent(GameObject* new_parent)
{
	float3 new_local = GetBoundingBox().CenterPoint() - new_parent->GetBoundingBox().CenterPoint();
	ComponentTransform* trans = (ComponentTransform*)GetComponent(COMPONENT_TRANSFORM); 
	trans->SetLocalPosition(new_local); 

	new_parent->PushChild(this);
	parent = new_parent;	
}

void GameObject::DeleteParent()
{

	//Conserving pos if object deasigned but not deleted

	ComponentTransform* trans = (ComponentTransform*)GetComponent(COMPONENT_TRANSFORM); 
	
	if(trans != nullptr)
		trans->SetLocalTransformMat(trans->GetGlobalTransform()); 

	//Deasigning child from parent and parent ptr

	for (int i = 0; i < parent->child_list.size(); i++)
	{
		if (parent->child_list[i]->GetID() == this->GetID())
		{
			parent->child_list.erase(parent->child_list.begin() + i);
			parent = nullptr; 
			break; 
		}			
	}	
}
void GameObject::EraseChild(GameObject * child, bool send_child_to_root)
{
	for (vector<GameObject*>::iterator it = child_list.begin(); it != child_list.end(); ++it)
	{
		if ((*it) == child)
		{
			// Clean
			child->parent = nullptr;
			child_list.erase(it);

			// Add to root
			if (send_child_to_root)
			{
				App->scene_intro->GetRoot()->PushChild(child);

				child->transform = child->transform;
			}

			break;
		}
	}
}
GameObject * GameObject::GetSupreme()
{
	return nullptr;
}

void GameObject::GetChildWithUID(uint UID, GameObject * go) const
{
	for (int i = 0; i < child_list.size(); ++i)
	{
		if (child_list[i]->GetID() == UID)
		{
			go = child_list[i];
			break;
		}
		else
		{
			child_list[i]->GetChildWithUID(UID, go);
			if (go != nullptr)
				break;
		}
	}
}
uint GameObject::GetNewChildID() const
{
	return new_child_id;
}
void GameObject::SetNewChildID(uint new_id) 
{
	new_child_id = new_id;
}
GameObject * GameObject::Find(string name)
{
	GameObject* to_ret = nullptr; 

	if (this->name == name)
	{
		return this; 
	}
	else if (GetNumChilds() > 0)
	{
		for (int i = 0; i < GetNumChilds(); i++)
		{
			to_ret = child_list[i]->Find(name); 

			if (to_ret != nullptr)
				break; 
		}
	}

	return to_ret; 
}
void GameObject::PushComponent(Component* comp)
{
	if (IsRoot())
	{
		PushRootComponent(comp);
	}

	else if(GetComponent(comp->type) != nullptr)
	{
		switch (comp->type)
		{
		case COMPONENT_MATERIAL: 
			ComponentMaterial* mat = (ComponentMaterial*)GetComponent(COMPONENT_MATERIAL);
			
			mat->Set((ComponentMaterial*)comp);
			LOG("Component Material %d Substituted", mat->textures_id); 
			break; 
		}

		// If it already exists we modify it 
	
	}
	else // if not we push it to the component list 
	{
		component_list.push_back(comp);
		LOG("New component added succesfully to %s", name.c_str()); 
	}
			
}

Component * GameObject::AddEmptyComponent(component_type type)
{
	Component* new_comp = nullptr;

	switch (type)
	{
	case component_type::COMPONENT_NULL:
		break;
	case component_type::COMPONENT_TRANSFORM:
	{
		ComponentTransform* t = new ComponentTransform(this);
		component_list.push_back(t);
		new_comp = t;
		break;
	}
	case component_type::COMPONENT_MESH_RENDERER:
	{
		ComponentMeshRenderer* mr = new ComponentMeshRenderer(this);
		component_list.push_back(mr);
		new_comp = mr;
		break;
	}
	case component_type::COMPONENT_MATERIAL:
	{
		ComponentMaterial* cm = new ComponentMaterial(this);
		component_list.push_back(cm);
		new_comp = cm;
		break;
	}
	case component_type::COMPONENT_CAMERA:
	{
		ComponentCamera* cam = new ComponentCamera(this);
		component_list.push_back(cam);
		new_comp = cam;
	}
	break;
	default:
		break;
	}

	return new_comp;
}

void GameObject::DeleteComponent(component_type new_component)
{
	for (vector<Component*>::iterator it = component_list.begin(); it != component_list.end(); it++)
	{
		if ((*it)->type == new_component)
		{
			switch ((*it)->type)
			{
			case COMPONENT_MESH_RENDERER:
			{
				ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponent(COMPONENT_MESH_RENDERER);
				mr->Delete();
				it = component_list.erase(it);
				mr = nullptr; 
				return;
			}
			 

			case COMPONENT_MATERIAL:
			{
				ComponentMaterial* mat = (ComponentMaterial*)GetComponent(COMPONENT_MATERIAL);
				mat->Delete();
				it = component_list.erase(it);
				mat = nullptr; 
				return;
			}
			

			case COMPONENT_CAMERA:
			{
				ComponentCamera* cam = (ComponentCamera*)GetComponent(COMPONENT_CAMERA);
				cam->Delete();
				it = component_list.erase(it);
				cam = nullptr;
				return; 
			}
			
			}
		}

	}
}

void GameObject::PushRootComponent(Component * comp)
{
	for (int i = 0; i < child_list.size(); i++)
	{
		if (child_list[i]->GetComponent(COMPONENT_TRANSFORM) != nullptr)
		{
			child_list[i]->PushComponent(comp);
		}
	}
}

void GameObject::PushChild(GameObject * child)
{
	if (child != nullptr)
	{
		child->parent = this;
		child_list.push_back(child);
	}

}
GameObject* GameObject::AddChild()
{
	char name[20];
	sprintf_s(name, "GameObject %d", new_child_id++);

	GameObject* new_go = new GameObject();
	new_go->SetName(name);
	new_go->SetParent(this);
	child_list.push_back(new_go);
	return new_go;

}

const std::vector<GameObject*> GameObject::GetChilds() const
{
	return child_list;
}

bool Component::Enable()
{ 
	return true;
}

bool Component::Update()
{
	return true;
}

bool Component::Disable()
{
	return true;
}
void Component::OnSave(json_file & config) const
{
}
void Component::OnLoad(json_file* config)
{

}


bool Component::IsActive() const
{
	return active;
}
bool Component::SetActive(bool _active)
{
	active = _active;
	return active;
}
void Component::SetComponentParent(GameObject* _parent)
{
	gameobject = _parent; 
}

void Component::DeleteComponent()
{
}

void Component::Serialize(json_file * file)
{
}
void Component::Serialize(json_file  file)
{
}

void GameObject::GetChildByUID(uint UID, GameObject *& go) const
{
	for (int i = 0; i < child_list.size(); ++i)
	{
		if (child_list[i]->GetID() == UID)
		{
			go = child_list[i];
			break;
		}
		else
		{
			child_list[i]->GetChildByUID(UID, go);
			if (go != nullptr)
				break;
		}
	}
}


