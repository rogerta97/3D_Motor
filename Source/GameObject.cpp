#include "GameObject.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include <cmath>
#include "ComponentDefs.h"

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

void GameObject::SetStatic(bool _active)
{
	is_static = _active; 
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
	id = _id;
}

uint GameObject::GetID()const
{
	return id;
}

GameObject::GameObject()
{

	parent = nullptr;
	active = true; 
	is_static = false; 
	is_root = false; 

	ComponentTransform* trans = new ComponentTransform(this);
	PushComponent(trans);

	trans->SetIdentityTransform();
	trans->type = COMPONENT_TRANSFORM;

}

void GameObject::Draw()
{
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

bool GameObject::IsRoot() const
{
	return is_root;;
}

void GameObject::SetRoot(const bool & _root)
{
	is_root = _root; 
}

uint GameObject::GetNumChilds()const
{
	return child_list.size();
}

uint GameObject::GetNumComponents()const
{
	return component_list.size();
}

GameObject * GameObject::GetParent()const
{
	return parent;
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
		LOG("New component added succesfully to %s", name); 
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
	child->parent = this;
	child_list.push_back(child); 
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
	parent = _parent; 
}

