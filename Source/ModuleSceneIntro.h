#pragma once
#include "Module.h"
#include "Globals.h"
#include "Component.h"
#include "Primitive.h"

class GameObject; 
class ComponentCamera;
struct PhysBody3D;
struct PhysMotor3D;
class Cube1; 
enum mesh_shape; 
class Octree; 
class OctreeNode; 

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
	GameObject* CreateGameObject(const char* name);

	void PrintConfigData();
	void PrintInspectorData();
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void AddGameObject(GameObject* GO); 
	vector<GameObject*> GetList();
	vector<ComponentCamera*>GetCameraList();
	bool IsListEmpty(); 

	//p2DynArray<p2Point<int>> GetCollisions(); 
	void SetCurrentGO(uint id);
	GameObject* GetCurrentGO(); 


	Octree* GetOctree(); 

	void ClearGOList(); 

public:
	vector<ComponentCamera*> cameras_list;
	Octree* octree = nullptr;

private:

	PPlane main_plane; 
	vector<GameObject*> GO_list;
	uint current_gameobject_id = 0;
	
};
