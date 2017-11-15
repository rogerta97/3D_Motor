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

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	// GO Management
	void AddGameObject(GameObject* GO);
	GameObject* GetGameObject(uint id);
	GameObject* CreateGameObject(const char* name);

	void SetCurrentGO(uint id);
	GameObject* GetCurrentGO();

	// Lists management
	void ClearGOList();
	vector<GameObject*> GetList();
	vector<ComponentCamera*>GetCameraList();

	void FromDynamicToStatic(GameObject* to_change);
	void FromStaticToDynamic(GameObject* to_change);

	int IsInDynamic(GameObject* to_check);
	int IsInStatic(GameObject* to_check);

	// UI Data
	void PrintConfigData();
	void PrintInspectorData();

	// Scene Management
	bool IsSceneEmpty(); 

public:

	vector<ComponentCamera*> cameras_list;
	Octree* octree = nullptr;

private:

	PPlane main_plane; 

	vector<GameObject*> GO_list;
	vector<GameObject*> static_GO_list;

	uint current_gameobject_id = 0;
	
};
