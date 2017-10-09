#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "p2Point.h"
#include "OpenGL.h"
#include "Element.h"

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
	
	//App->renderer3D->GenVertexArrayBuffer("array"); 

	// DrawArrays

	//App->renderer3D->FillBufferData(vertices, 36); 

	//App->renderer3D->GenVertexArrayBuffer("element"); 

	////DrawElements
	//origin = float3(5.0f, 5.0f, 5.0f);

	//float vertices2[8*3] =
	//{
	//	origin.x, origin.y + 1, origin.z,			
	//	origin.x + 1, origin.y + 1, origin.z,		
	//	origin.x, origin.y, origin.z,

	//	origin.x + 1, origin.y, origin.z,
	//	origin.x + 1, origin.y + 1, origin.z + 1,
	//	origin.x + 1, origin.y, origin.z + 1,

	//	origin.x, origin.y + 1, origin.z + 1,
	//	origin.x, origin.y, origin.z + 1,
	//};

	//App->renderer3D->FillBufferData(vertices2, 24); 



	my_cube.Start(); 
	my_cube2.Start(); 
	sphere.Start(5, 9, 9); 

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
	
	//App->renderer3D->DrawCubeDirectMode(); 
	my_cube.Draw(); 
	my_cube2.Draw(); 
	sphere.Draw(); 

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





