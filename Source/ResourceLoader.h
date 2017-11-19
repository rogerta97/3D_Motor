#ifndef _RESOURCE_LOADER_H_
#define _RESOURCE_LOADER_H_
#include "Module.h"
#include <map>

class ComponentMaterial;
class ComponentMeshRenderer;
class GameObject;

enum resource_t
{
	mesh_t,
	material_t,
	scene_t,
	null_t
};
class Resource {
public:
	Resource(GameObject* owner_) ;
	~Resource();
	bool Active(bool active);
	bool IsActive()const;
	GameObject* GetOwner()const;
	void SetType(resource_t t);
	resource_t GetType()const;
public:
	resource_t type = null_t;
	uint uid = 0;
	GameObject* owner = nullptr;
	bool active = false;
};
class ResourceLoader : public Module {
public:
	ResourceLoader(bool start_enable = true);
	 ~ResourceLoader() ;

	 bool Start();
	 update_status Update(float dt);
	 bool CleanUp();

	 void AddResource(uint uid_key, Resource* resource);

	 void Load(const char* path);
	 bool Exists(std::string& file, int file_id = -1);
public:
	std::map<uint, Resource*> resources;

};
#endif !_RESOURCE_LOADER_H_
