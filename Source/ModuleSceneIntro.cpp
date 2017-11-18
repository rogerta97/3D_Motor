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
#include "ModuleFBXLoader.h"
#include "ModuleCamera3D.h"
#include "ModuleRenderer3D.h"
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

	srand(time(NULL));

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

// Update
update_status ModuleSceneIntro::Update(float dt)
{
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
			//octree->CollectIntersections(oct_candidates, App->camera->GetMainCam()->frustum); 
			//to_draw = MapToVector(oct_candidates); 
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
	for (int i = 0; i < unique_id; i++)
	{
		if (GO_list[i]->GetID() == unique_id)
			return GO_list[i]; 
	}

	return nullptr; 
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

//float3 ModuleSceneIntro::GetBBPointFromStaticList(int point_type) //0 will search for the min point and 1 for the max point of any object in static list
//{
//
//	float3 ret; 
//
//	switch (point_type)
//	{
//	case 0: 
//		ret = static_GO_list[0]->GetBoundingBox().minPoint; 
//
//		for (int i = 0; i < static_GO_list.size(); i++)
//		{		
//			if (ret.Length() > static_GO_list[i]->GetBoundingBox().minPoint.Length())
//			{
//				ret = static_GO_list[i]->GetBoundingBox().minPoint; 
//			}
//		}
//
//		break; 
//
//	case 1:
//		ret = static_GO_list[0]->GetBoundingBox().maxPoint;
//
//		for (int i = 0; i < static_GO_list.size(); i++)
//		{
//			if (ret.Length() < static_GO_list[i]->GetBoundingBox().maxPoint.Length())
//			{
//				ret = static_GO_list[i]->GetBoundingBox().maxPoint;
//			}
//		}
//		break; 
//	}
//
//	return ret;
//}

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







