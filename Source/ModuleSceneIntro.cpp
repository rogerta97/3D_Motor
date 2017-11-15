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

GameObject* ModuleSceneIntro::GetGameObject(uint id)
{
	GameObject* ret = nullptr;

	uint i = 0;

	// Check in Dynamic List

	for (std::vector<GameObject*>::iterator it = scene_GO_list.begin(); it != scene_GO_list.end(); it++)
	{
		if (i == id)
		{
			ret = (*it);
			break;
		}
		++i;
	}	

	// Check in Static list 

	//for (std::vector<GameObject*>::iterator it2 = static_GO_list.begin(); it2 != static_GO_list.end(); it2++)
	//{
	//	if (i == id)
	//	{
	//		ret = (*it2);
	//		break;
	//	}
	//	++i;
	//}

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
	if(octree != nullptr)
		octree->DrawOctree();

	glBindTexture(GL_TEXTURE_2D, 0); 
	
	main_plane.Render();

	for (std::vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); it++)
	{
		//ComponentCamera* camera = (ComponentCamera*)(*it)->GetComponent(COMPONENT_CAMERA);
		(*it)->Draw(); 

		//if (camera != nullptr && camera->IsActive())
		//{
		//	App->renderer3D->curr_cam->DrawFrustum(camera->frustum,White);
		//	//App->renderer3D->rendering_cameras.push_back(camera);
		//}
	}

	for (std::vector<GameObject*>::iterator it2 = static_GO_list.begin(); it2 != static_GO_list.end(); it2++)
	{
		//ComponentCamera* camera = (ComponentCamera*)(*it2)->GetComponent(COMPONENT_CAMERA);
		(*it2)->Draw();

		//if (camera != nullptr && camera->IsActive())
		//{
		//	App->renderer3D->curr_cam->DrawFrustum(camera->frustum, White);
		//	//App->renderer3D->rendering_cameras.push_back(camera);
		//}
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
	scene_GO_list.push_back(GO); 

	GO->SetID(GetGameObjectsNum() - 1);
	current_gameobject_id = GO->GetID();
}

vector<GameObject*> ModuleSceneIntro::GetSceneList()
{
	return scene_GO_list;
}

vector<ComponentCamera*> ModuleSceneIntro::GetCameraList()
{
	return cameras_list;
}

void ModuleSceneIntro::FromDynamicToStatic(GameObject* to_change)
{

	if (IsInDynamic(to_change->GetID()) != -1)
	{
		int pos = IsInDynamic(to_change->GetID());

		GO_list.erase(GO_list.begin() + pos);
		static_GO_list.push_back(to_change); 

		if (IsInDynamic(to_change->GetID()) == -1 && IsInStatic(to_change->GetID()) != -1)
			LOG("%s moved to static list", to_change->GetName()); 
	}	
}

void ModuleSceneIntro::FromStaticToDynamic(GameObject* to_change)
{
	if (IsInStatic(to_change->GetID()) != -1)
	{
		int pos = IsInStatic(to_change->GetID());

		static_GO_list.erase(static_GO_list.begin() + pos);
		GO_list.push_back(to_change);

		if (IsInStatic(to_change->GetID()) == -1 && IsInDynamic(to_change->GetID()) != -1)
			LOG("%s moved to dynamic list", to_change->GetName());
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
	return GO_list.size() + static_GO_list.size(); 
}


void ModuleSceneIntro::SetCurrentGO(uint id)
{
	current_gameobject_id = id;
}

GameObject* ModuleSceneIntro::GetCurrentGO()
{

	if (IsInDynamic(current_gameobject_id) != -1)
		return GO_list.at(IsInDynamic(current_gameobject_id));

	else if(IsInStatic(current_gameobject_id) != -1)
		return static_GO_list.at(IsInStatic(current_gameobject_id));;
}


void ModuleSceneIntro::ClearSceneGOList()
{
	scene_GO_list.clear();
}





