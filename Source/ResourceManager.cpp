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
	return true;
}

update_status ResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ResourceManager::CleanUp()
{
	bool ret = true;

	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end();)
	{
		RELEASE(it->second);

		it = resources.erase(it);
	}

	return ret;
}

void ResourceManager::Load(const char * path)
{
	file_format format = FF_NULL;

	format = (file_format)GetPathTermination(path);

	if (format != FF_NULL)
	{
		if (format == FF_FBX)
		{
			LOG("FBX file dragged to window");
			App->CopyFileTo(path, "Assets\\Meshes");
			//new import
			//App->mesh_importer->ImportFile(file.c_str());
			//old code
			meshes_loader->LoadFileScene(path);
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

bool ResourceManager::Exists(std::string & file, int file_id)
{
	bool ret = false;

	uint cut = file.find_last_of(".");
	std::string file_name = file.substr(0, cut);

	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		//check if the resouce already exists
		//take care of considering that FBX has more than one resource
	}

	return ret;
}

Resource::Resource(GameObject * owner_)
{
	owner = owner_;
	active = true;//default active when created
}

Resource::~Resource()
{
}

bool Resource::Active(bool active)
{
	this->active = active;
	return this->active;
}

bool Resource::IsActive() const
{
	return active;
}

GameObject * Resource::GetOwner() const
{
	if (owner != nullptr)
		return owner;
	return nullptr;
}

void Resource::SetType(resource_t t)
{
	type = t;
}

resource_t Resource::GetType() const
{
	return type;
}


