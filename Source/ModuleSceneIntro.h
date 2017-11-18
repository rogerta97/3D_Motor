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
	void DeleteGameObject(GameObject* GO); 
	void DeleteGameObjectsNow(); 

	GameObject* Find(const int& unique_id) const; 

	GameObject* GetGameObject(uint position);
	GameObject* CreateGameObject(const char* name);

	void SetCurrentGO(uint id);
	GameObject* GetCurrentGO();

	// Lists management
	vector<GameObject*> GetDynamicGOList();
	vector<GameObject*> GetStaticGOList();

	GameObject* GetStaticGO(int unique_id);

	vector<ComponentCamera*>GetCameraList();

	void AddToList(GameObject* GO); 
	void DeleteFromList(GameObject* GO);

	void AddToStaticRecursive(GameObject* to_change);
	void AddToStatic(GameObject* to_change);

	void DeleteFromStaticRecursive(GameObject* to_change);
	void DeleteFromStatic(GameObject* to_change);

	int IsInDynamic(const int& to_search);
	int IsInStatic(const int& to_search);

	int GetGameObjectsNum(); 

	// UI Data
	void PrintConfigData();
	void PrintInspectorData();


	// Scene Management
	bool IsSceneEmpty(); 
	GameObject* GetFarestObjectFrom(float3 origin);


public:

	vector<ComponentCamera*> cameras_list;
	Octree* octree = nullptr;

private:

	PPlane main_plane; 

	vector<GameObject*> GO_list;
	vector<GameObject*> static_GO_list;

	vector<GameObject*> to_delete; 

	uint current_gameobject_id = 0;

	
};

