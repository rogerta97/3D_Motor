#ifndef _RESOURCE_LOADER_H_
#define _RESOURCE_LOADER_H_
#include "Module.h"
#include <map>
#include <list>

class ComponentMaterial;
class ComponentMeshRenderer;
class Component; 
class GameObject;

class ResourceMeshLoader;
class ResourceMaterialLoader;

enum resource_t
{
	RESOURCE_TYPE_MESH,
	RESOURCE_TYPE_MATERIAL,
	RESOURCE_TYPE_SCENE,
	RESOURCE_TYPE_NULL
};
class Resource
{
public: 
	Resource(const char* name, resource_t type, Component* resource_cmp);
	~Resource(); 

	virtual void Init(); 
	std::string GetName() const; 

	resource_t GetType() const; 
	Component* GetData() const; 

	int GetResourceID() const;
	void SetResourceID(int id); 

	int id;

private: 
	std::string name; 
	resource_t type;

	Component* resource_data; 
};

class ResourceManager : public Module {
public:
	ResourceManager(bool start_enable = true);
	 ~ResourceManager() ;

	 bool Start();
	 update_status Update(float dt);
	 bool CleanUp();

	 //Resource management
	 void AddResource(Resource* resource);
	 Resource* GetResource(const char* name); 
	 bool Exist(std::string file_name);

	 void Load(const char* path);
	
public:
	uint curr_id; 

	list<Resource*> resources;

	ResourceMeshLoader*	 	 meshes_loader;
	ResourceMaterialLoader*  material_loader;

};
#endif !_RESOURCE_LOADER_H_
