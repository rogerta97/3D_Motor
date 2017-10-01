#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "p2Point.h"
#include "OpenGL.h"

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

	first_cube.size = vec3(1,1,1); 
	first_cube.SetPos(0, 0, 0); 

	first_sphere.radius = 1; 
	first_sphere.SetPos(0, 0, 5); 

	obj_list.push_back(first_sphere); 
	obj_list.push_back(first_cube);

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
	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	
	first_cube.Render(); 
	first_sphere.Render(); 

	return UPDATE_CONTINUE; 
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{

}





