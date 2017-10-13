#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "p2Point.h"
#include "OpenGL.h"
#include "Gizmo.h"
#include "ModuleFBXLoader.h"

#include "PhysBody3D.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	name = "Scene"; 
	App->performance.InitTimer(name);

	LOG("Loading Intro assets");
	bool ret = true;

	srand(time(NULL));

	

	App->performance.SaveInitData(name); 

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	
	if (!App->fbx_loader->GetList().empty())
	{
		for (std::list<GLGizmo*>::iterator it = App->fbx_loader->GetList().begin(); it != App->fbx_loader->GetList().end(); it++)
		{
			obj_list.push_back((*it)); 
		}

		App->fbx_loader->GetList().clear(); 
	}

	for (std::vector<Gizmo*>::iterator it = obj_list.begin(); it != obj_list.end(); it++)
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

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}





