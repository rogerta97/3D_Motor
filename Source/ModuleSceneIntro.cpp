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
	//App->camera->(initial_pos);
	float3 initial_look_at(0, 0, 0);
	//App->camera->LookAt(initial_look_at);
	App->performance.SaveInitData(name); 

	main_plane = PPlane(0, 1, 0, 0);
	main_plane.axis = true;

	AABB root_octree_box(vec(-100, -100, -100), vec(100, 100, 100));
	
	octree = new Octree();
	octree->Create(root_octree_box);

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
	for (std::vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); it++)
	{
		if (i == id)
		{
			ret = (*it);
			break;
		}
		++i;
	}	
	return ret;
}

GameObject * ModuleSceneIntro::CreateGameObject(const char * name, mesh_shape shape)
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
	octree->DrawOctree();
	glBindTexture(GL_TEXTURE_2D, 0); 
	
	main_plane.Render();

	for (std::vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); it++)
	{
		(*it)->Draw(); 
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

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}

void ModuleSceneIntro::AddGameObject(GameObject * GO)
{

	GO_list.push_back(GO); 

	GO->SetID(GO_list.size() - 1);
	current_gameobject_id = GO->GetID();
}

vector<GameObject*> ModuleSceneIntro::GetList()
{
	return GO_list;
}

bool ModuleSceneIntro::IsListEmpty()
{
	return GO_list.empty();
}

void ModuleSceneIntro::SetCurrentGO(uint id)
{
	current_gameobject_id = id;
}

GameObject* ModuleSceneIntro::GetCurrentGO()
{
	return GO_list.at(current_gameobject_id);
}

void ModuleSceneIntro::ClearGOList()
{
	GO_list.clear();
}





