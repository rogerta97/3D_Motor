#include "Module.h"
#include "ResourceManager.h"
#include "Application.h"
#include "JSON.h"
#include "GameObject.h"
#include "Globals.h"
#include "Functions.h"

#include "ResourceMaterialLoader.h"
#include "ResourceMeshLoader.h"

ResourceManager::ResourceManager(bool start_enable) : Module(start_enable)
{
}

ResourceManager::~ResourceManager()
{
}

bool ResourceManager::Start()
{

	meshes_loader = new ResourceMeshLoader(); 
	material_loader = new ResourceMaterialLoader();

	meshes_loader->Start(); 
	material_loader->Start(); 

	//resources.push_back(meshes_loader); 
	//resources.push_back(material_loader);

	//for (list<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	//{
	//	(*it)->Init(); 
	//}

	return true;
}

update_status ResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
	bool ret = true;

	for (list<Resource*>::iterator it = resources.begin(); it != resources.end();)
	{
		it = resources.erase(it);
	}

	return ret;
}

void ResourceManager::AddResource(uint uid_key, Resource * resource)
{
}

Resource * ResourceManager::GetResource(std::string name)
{
	for (list<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it)->GetName() == name)
			return (*it);
	}

	return nullptr;
}

bool ResourceManager::Exist(std::string file_name)
{
	for (list<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it)->GetName() == file_name)
			return true; 
	}

	return false; 
}

void ResourceManager::Load(const char * path)
{
	file_format format = FF_NULL;

	format = (file_format)GetPathTermination(path);
	std::string file_name = GetLastPathCommand(path, false); 

	if (format != FF_NULL)
	{
		if (format == FF_FBX)
		{
			LOG("FBX file dragged to window");
			App->CopyFileTo(path, "Assets\\Meshes");

			if (Exist(file_name.c_str()))
			{
				
			}
			else
			{
				meshes_loader->LoadFileScene(path);
			}
			//new import
			//App->mesh_importer->ImportFile(file.c_str());
			//old code
			
		}

		if (format == FF_PNG || format == FF_TGA)
		{
			//For now we set the texture in the last obect created (the unique in the scene) 
			LOG("PNG file dragged to window");

			App->CopyFileTo(path, "Assets\\Materials");
			//new importfytfydtrdrdut
			material_loader->ImportImage(path);
		}
		else
			LOG("No gameobject selected where PNG can be dragged");
	}
	else
		LOG("Error getting the path");
}


//Resource::Resource(GameObject * owner_)
//{
//	owner = owner_;
//	active = true;//default active when created
//}

Resource::Resource(std::string name, resource_t type, Component* resource_cmp)
{
	this->name = name; 
	this->type = type; 
	this->resource_data = resource_cmp; 
}

Resource::~Resource()
{
}

void Resource::Init()
{
}
std::string Resource::GetName() const
{
	return name;
}

//void Resource::Init()
//{
//
//}
//
//bool Resource::Exist(std::string path)
//{
//	return true;
//}
//
//bool Resource::Active(bool active)
//{
//	this->active = active;
//	return this->active;
//}
//
//bool Resource::IsActive() const
//{
//	return active;
//}
//
//GameObject * Resource::GetOwner() const
//{
//	if (owner != nullptr)
//		return owner;
//	return nullptr;
//}
//
//void Resource::SetType(resource_t t)
//{
//	type = t;
//}
//
//resource_t Resource::GetType() const
//{
//	return type;
//}


