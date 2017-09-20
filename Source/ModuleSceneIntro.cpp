#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "p2Point.h"

#include "PhysBody3D.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	srand(time(NULL));


	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	main_plane = PPlane(0, 1, 0, 0);
	main_plane.axis = true;

	Sphere s1;
	s1.pos = vec(0.0f, 0.0f, 0.0f);
	s1.r = 5;

	Sphere s2;
	s2.pos = vec(0.0f, 0.0f, 0.0f);
	s2.r = 3;

	object_list.PushBack(s1);
	object_list.PushBack(s2);


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

	CONSOLELOG("Box %d collided with box %d");

	main_plane.Render();

	//p2DynArray<vec2> collisions = GetCollisions();
	
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

//p2DynArray<p2Point<int>> ModuleSceneIntro::GetCollisions()
//{
//	p2DynArray<p2Point<int>> collision_list; 
//	/*
//	int i = 0, z = 0; 
//
//	while (i < object_list.Count())
//	{
//		while (z < object_list.Count())
//		{
//			if (z == i)
//			{
//				z++;
//				continue;
//			}
//				 
//			bool test = false;
//			test = object_list[i].Intersects(object_list[z]);
//
//			if (test)
//			{
//				p2Point<int> new_collision;
//
//				new_collision.x = i; 
//				new_collision.y = z; 
//
//				collision_list.PushBack(new_collision); 
//			}
//			z++;				
//		}
//
//		z = 0; 
//		i++; 
//	}
//*/
//	return collision_list;
//}




