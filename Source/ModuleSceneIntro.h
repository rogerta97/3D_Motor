#pragma once
#include "Module.h"
#include "Globals.h"

class GameObject; 
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
	GameObject* GetGameObject(uint id);

	void PrintConfigData();
	void PrintInspectorData();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void AddGameObject(GameObject* GO); 

	//p2DynArray<p2Point<int>> GetCollisions(); 
	void ClearGOList(); 
	vector<GameObject*> GO_list;

private:  
};
