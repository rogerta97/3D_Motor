#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "p2Point.h"
#include "Component.h"
#include "ComponentDefs.h"
#include "OpenGL.h"
#include "GameObject.h"
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

	GameObject* GO = CreateGameObject("Camara Holder", EMPTY_MESH);

	ComponentCamera* new_cam = new ComponentCamera();
	GO->PushComponent(new_cam); 
	new_cam->SetActive(true);

	App->performance.SaveInitData(name); 

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
	GameObject* new_go = new GameObject(shape); 
	new_go->SetName(name); 

	LOG("GameObject '%s' created", name); 

	GO_list.push_back(new_go); 

	return new_go; 

}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	
	for (std::vector<GameObject*>::iterator it = GO_list.begin(); it != GO_list.end(); it++)
	{
		(*it)->Draw(); 
	}
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	//App->renderer3D->PrintBufferData(); 

	//int indices[36] =
	//{
	//	0, 3, 2,
	//	0, 1, 3,
	//	1, 4, 3, 
	//	4, 5, 3,
	//};

	//App->renderer3D->PrintBufferData(&indices[0]); 
	
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
}

vector<GameObject*> ModuleSceneIntro::GetList()
{
	return GO_list;
}

bool ModuleSceneIntro::IsListEmpty()
{
	return GO_list.empty();
}

void ModuleSceneIntro::ClearGOList()
{
	GO_list.clear();
}





