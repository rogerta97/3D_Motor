#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "p2Point.h"
#include "Component.h"
#include "ComponentDefs.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Octree.h"
#include "ResourceMeshLoader.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
#include "UID.h"
#include "SceneImporter.h"
#include "Functions.h"

#include "PhysBody3D.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled)
{

}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	name = "Scene"; 
	App->performance.InitTimer(name);

	LOG("Loading Intro assets");
	bool ret = true;

	root = new GameObject();
	root->Start();
	root->SetName("Root");
	srand(time(NULL));
	GO_list.push_back(root);

	float3 initial_pos(0.f, 10.f, 10.f);
	App->camera->SetCamPosition(initial_pos);
	float3 initial_look_at(0, 0, 0);
	App->camera->LookAt(initial_look_at);

	App->performance.SaveInitData(name); 

	main_plane = PPlane(0, 1, 0, 0);
	main_plane.axis = true;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

GameObject* ModuleSceneIntro::GetGameObject(uint pos)
{
	GameObject* ret = nullptr;

	uint i = 0;

	for (std::vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); it++)
	{
		if (i == pos)
		{
			ret = (*it);
			break;
		}

		++i;
	}	

	return ret;
}



GameObject * ModuleSceneIntro::CreateGameObject(const char * name)
{
	GameObject* new_go = new GameObject(); 
	new_go->SetName(name); 

	LOG("GameObject '%s' created", name); 

	AddGameObject(new_go); 

	return new_go; 

}

GameObject * ModuleSceneIntro::GetGameObjectWithUID(uint unique_id)
{
	GameObject* ret = nullptr;

	for (int i = 0; i < GO_list.size(); ++i)
	{
		if (GO_list[i]->GetID() == unique_id)
		{
			ret = GO_list[i];
			break;
		}
		else
		{
			GO_list[i]->GetChildWithUID(unique_id, ret);
			if (ret != nullptr)
				break;
		}
	}

	return ret;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	//Fireworks 
	Firework new_firework;

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		new_firework.ShootFireWork(); 
		fireworks_active.push_back(new_firework); 
	}

	if (fireworks_active.empty() == false)
		new_firework.UpdateFireWork(); 
		
	vector<GameObject*> to_draw; 

	bool culling = false; 

	if(App->camera->HasMainCamera())
		culling = App->camera->GetMainCam()->frustum_culling;

	if(octree != nullptr)
		octree->DrawOctree();

	glBindTexture(GL_TEXTURE_2D, 0); 
	
	main_plane.Render();

	if (App->camera->HasMainCamera() && culling)
	{
		if (octree == nullptr)
		{
			App->camera->GetMainCam()->GetObjectsOnFrustum(to_draw);
			to_draw.push_back(App->camera->GetMainCam()->GetComponentParent());
		}
		else
		{
			map<float, GameObject*> oct_candidates;
		}			
	}
	else
		to_draw = GO_list; 


	//Draw just supreme parents
	if (culling == false)
	{
		for (std::vector<GameObject*>::iterator it = to_draw.begin(); it != to_draw.end(); it++)
		{
			if ((*it)->GetParent() == nullptr)
				(*it)->Draw();
		}
	}
	else
	{
		for (std::vector<GameObject*>::iterator it = to_draw.begin(); it != to_draw.end(); it++)
		{
				(*it)->Draw();
		}
	}


	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	
	return UPDATE_CONTINUE; 
}
void ModuleSceneIntro::PrintConfigData()
{

}
void ModuleSceneIntro::PrintInspectorData()
{

}

bool ModuleSceneIntro::IsSceneEmpty()
{
	return (GO_list.empty() && static_GO_list.empty()); 
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

void ModuleSceneIntro::AddGameObject(GameObject * GO)
{
	GO_list.push_back(GO); 
	current_gameobject_id = GO->GetID();
}

void ModuleSceneIntro::DeleteGameObject(GameObject * GO)
{
	if (GO->IsStatic())
	{
		DeleteFromStatic(GO); 
	}

	DeleteFromList(GO); 

	if (!GO->child_list.empty())
	{
		for (vector<GameObject*>::iterator it = GO->child_list.begin(); it != GO->child_list.end(); it++)
		{
			DeleteFromList((*it));
			to_delete.push_back((*it)); 
		}
	}

	to_delete.push_back(GO); 

	DeleteGameObjectsNow(); 
}

void ModuleSceneIntro::DeleteGameObjectsNow()
{
	for (vector<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); it++)
	{
		(*it)->Delete(); 
		delete((*it));
		(*it) = nullptr;
	}

	to_delete.clear(); 
	SetCurrentGO(-1);
}

GameObject* ModuleSceneIntro::Find(const int& unique_id) const
{
	for (int i = 0; i < GO_list.size(); i++)
	{
		int aux = GO_list[i]->GetID();
		if (GO_list[i]->GetID() == unique_id)
			return GO_list[i]; 
	}

	return nullptr; 
}

GameObject * ModuleSceneIntro::FindByNameRecursive(string name) const
{
	GameObject* to_ret = nullptr; 

	for (int i = 0; i < GO_list.size(); i++)
	{
		to_ret = GO_list[i]->Find(name);

		if (to_ret != nullptr)
			break; 
	}

	return to_ret; 
}

string ModuleSceneIntro::GetNameFromPath(const char * path)
{
	string ret;

	for (int i = 0; path[i] != '\0'; i++)
	{
		if (path[i] == '\\' || path[i] == '/')
		{
			ret.clear();
			continue;
		}

		ret += path[i];
	}

	return ret;
}

vector<GameObject*> ModuleSceneIntro::GetDynamicGOList()
{
	return GO_list;
}

vector<GameObject*> ModuleSceneIntro::GetStaticGOList()
{
	return static_GO_list;
}

GameObject * ModuleSceneIntro::GetStaticGO(int unique_id)
{
	GameObject* ret = Find(unique_id); 

	if (ret->IsStatic()) 
		return ret; 
	else 
		return nullptr; 
}

GameObject * ModuleSceneIntro::GetFarestObjectFrom(float3 origin)
{

	GameObject* to_ret = nullptr; 

	if (GetGameObjectsNum() != 0)
	{
		float distance = GetGameObject(0)->GetBoundingBox().Distance(origin);
		to_ret = GetGameObject(0); 

		for (int i = 0; i < GetGameObjectsNum(); i++)
		{
			if (distance < GetGameObject(i)->GetBoundingBox().Distance(origin))
			{
				distance = GetGameObject(i)->GetBoundingBox().Distance(origin);
				to_ret = GetGameObject(i); 
			}
				
		}
	}

	return to_ret;
}

void ModuleSceneIntro::RecursiveGetGameObjectTree(GameObject * go, vector<GameObject*>& fill)
{
	fill.push_back(go);

	vector<GameObject*> childs = go->child_list;
	for (vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		RecursiveGetGameObjectTree(*it, fill);
	}
}

GameObject * ModuleSceneIntro::GetRoot() const
{
	if (root == nullptr)

		LOG("root is Null");


	return root;
}

void ModuleSceneIntro::Serialize(json_file * file)
{
	for (std::vector<GameObject*>::iterator go = GO_list.begin(); go != GO_list.end(); ++go)
	{
		(*go)->Serialize(file);
	}
}
void ModuleSceneIntro::Serialize(json_file  file)
{
	for (std::vector<GameObject*>::iterator go = GO_list.begin(); go != GO_list.end(); ++go)
	{
		(*go)->Serialize(file);
	}
}

void ModuleSceneIntro::ClearSelection()
{

}

void ModuleSceneIntro::DestroyAllGameObjectsNow()
{
	for (vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); ++it)
	{
		Destroy((*it));
	}

		for (vector<GameObject*>::iterator to_del = to_delete.begin(); to_del != to_delete.end();)
		{
			// Add childs to delete
			vector<GameObject*> childs = (*to_del)->GetChilds();
			for (vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end(); ++ch)
			{
				(*ch)->parent = nullptr;
				Destroy(*ch);
			}

			// Reset parent
			if ((*to_del)->GetParent() != nullptr)
			{
				(*to_del)->GetParent()->EraseChild(*to_del, false);
			}

			// Delete from list
			for (vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end();)
			{
				if ((*to_del) == (*it))
				{
					it = GO_list.erase(it);
					break;
				}
				else
					++it;
			}



			// Free
			(*to_del)->Delete();
			delete (*to_del);

			to_del = to_delete.erase(to_del);
		}
	}



void ModuleSceneIntro::Destroy(GameObject * go)
{
	{
		for (vector<GameObject*>::iterator it = to_delete.begin(); it != to_delete.end(); ++it)
		{
			if (go == (*it))
				return;
		}

		to_delete.push_back(go);
	}
}

vector<ComponentCamera*> ModuleSceneIntro::GetCameraList()
{
	return cameras_list;
}

void ModuleSceneIntro::AddToList(GameObject * GO)
{
	GO_list.push_back(GO);
}

void ModuleSceneIntro::DeleteFromList(GameObject * GO)
{
	for (std::vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); it++)
	{
		if ((*it)->GetID() == GO->GetID())
		{
			it = GO_list.erase(it); 
			return;
		}
	}
}

void ModuleSceneIntro::AddToStaticRecursive(GameObject* to_change)
{
	AddToStatic(to_change);

	if(!to_change->child_list.empty())

		for (int i = 0; i < to_change->child_list.size(); i++)
		{
			AddToStaticRecursive(to_change->child_list[i]);
		}
		
}

void ModuleSceneIntro::AddToStatic(GameObject* to_change)
{
	static_GO_list.push_back(to_change);
	LOG("%s added to static list", to_change->GetName()); 
}

void ModuleSceneIntro::DeleteFromStaticRecursive(GameObject* to_del)
{
	DeleteFromStatic(to_del);

	if (!to_del->child_list.empty())

		for (int i = 0; i < to_del->child_list.size(); i++)
		{
			DeleteFromStaticRecursive(to_del->child_list[i]);	
		}
}

void ModuleSceneIntro::DeleteFromStatic(GameObject* to_change)
{
	for (vector<GameObject*>::iterator it = static_GO_list.begin(); it != static_GO_list.end(); it++)
	{
		if ((*it)->GetID() == to_change->GetID())
		{
			LOG("%s deleted from static list", to_change->GetName());
			LOG("%d", to_change->GetID());
			static_GO_list.erase(it); 

			if (static_GO_list.empty())
				break; 		
		}

	}
}

int ModuleSceneIntro::IsInDynamic(const int& searched_id)
{
	for (int i = 0; i < GO_list.size(); i++)
	{
		if (searched_id == GO_list[i]->GetID())
			return i; 
	}

	return -1; 
}

int ModuleSceneIntro::IsInStatic(const int& searched_id)
{
	for (int i = 0; i < static_GO_list.size(); i++)
	{
		if (searched_id == static_GO_list[i]->GetID())
			return i;
	}

	return -1;
}


int ModuleSceneIntro::GetGameObjectsNum()
{
	return GO_list.size(); 
}


void ModuleSceneIntro::SetCurrentGO(uint id)
{

	if (App->camera->IsLooking4Camera())
	{
		ComponentCamera* cam = (ComponentCamera*)Find(id)->GetComponent(COMPONENT_CAMERA);

		if (cam != nullptr)
		{
			App->camera->AssignNewMainCamera(cam);
		}
	}

	current_gameobject_id = id;
}

GameObject* ModuleSceneIntro::GetCurrentGO()
{
	GameObject* ret = nullptr; 

	if (current_gameobject_id != -1)
		ret = Find(current_gameobject_id); 
	
	return ret; 
}

void ModuleSceneIntro::ClearRelations()
{
	relations.clear();
}

void ModuleSceneIntro::AddRelationGo(GameObject * go)
{
	int id = relations.size();
	Relation rel(id, go);
	relations.push_back(rel);
}

void ModuleSceneIntro::AddRleationIdGo(int id, GameObject * go, int parent_id)
{
	if (parent_id == -1)
	{
		Relation rel(id, go);
		relations.push_back(rel);
	}
	else
	{
		Relation rel(id, go, parent_id);
		relations.push_back(rel);
	}
}

int ModuleSceneIntro::GetRelationGo(GameObject * go)
{
	for (vector<Relation>::iterator it = relations.begin(); it != relations.end(); ++it)
	{
		if ((*it).go == go)
		{
			return (*it).id;
		}
	}

	return -1;
}

GameObject * ModuleSceneIntro::GetRelationId(int id)
{
	for (vector<Relation>::iterator it = relations.begin(); it != relations.end(); ++it)
	{
		if ((*it).id == id)
		{
			return (*it).go;
		}
	}

	return nullptr;
}


GameObject * ModuleSceneIntro::CreateNewGO(std::string force_id)
{
	string new_id;

	if (force_id == "")
	{
		void* a = this;
		void** a_ptr = &a;
		uint size = sizeof(this);
		char* data = new char[size];
		memcpy(data, a_ptr, size);

		uint* uid = md5(data, size);
		new_id = *uid;
	}
	else
		new_id = force_id;

	GameObject* game_object = new GameObject();

	GO_list.push_back(game_object);
	root->PushChild(game_object);
	game_object->Start();

	return game_object;
}




