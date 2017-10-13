#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Gizmo.h"


struct PhysBody3D;
struct PhysMotor3D;
class Cube1; 

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt); 
	bool CleanUp();
	void PrintConfigData();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	//p2DynArray<p2Point<int>> GetCollisions(); 

	std::vector<Gizmo*> obj_list; 

private:  
};
