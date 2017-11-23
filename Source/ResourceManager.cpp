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

	curr_id = 0; 

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

void ResourceManager::AddResource(Resource * resource)
{
	switch (resource->GetType())
	{
	case RESOURCE_TYPE_MESH: 
		meshes_loader->meshes_loaded.insert(std::pair<uint, ComponentMeshRenderer*>(resource->id, (ComponentMeshRenderer*)resource->GetData()));
		LOG("Resource MESH %s added to folder", resource->GetData()->GetComponentParent()->GetName());
		break; 

	case RESOURCE_TYPE_MATERIAL:
		material_loader->materials_loaded.insert(std::pair<uint, ComponentMaterial*>(resource->id, (ComponentMaterial*)resource->GetData()));
		LOG("Resource MATERIAL %s added to folder", resource->GetData()->GetComponentParent()->GetName());
		break;
	}

	resources.push_back(resource);
	curr_id++; 	
}


Resource* ResourceManager::GetResource(const char* name)
{
	//We corresponding resource from its name
	Resource* curr_resource = nullptr; 

	for (list<Resource*>::iterator it = resources.begin(); it != resources.end(); it++)
	{
		if ((*it)->GetName() == name)
		{
			curr_resource = (*it);
			break;
		}			
	}

	////We get the ID associated with it 
	//if (curr_resource != nullptr)
	//{
	//	int id = curr_resource->GetResourceID(); 

	//	//We look for its corresponding component in the lists

	//	switch (curr_resource->GetType())
	//	{
	//	case RESOURCE_TYPE_MESH:
	//	{
	//		ComponentMeshRenderer* to_ret = meshes_loader->GetComponentFromID(curr_resource->id);
	//		return (Component*)to_ret;
	//	}
	//		
	//	case RESOURCE_TYPE_MATERIAL:
	//	{
	//		ComponentMaterial* to_ret = material_loader->GetComponentFromID(curr_resource->id);
	//		return (Component*)to_ret;
	//	}
	//	
	//	}
	//}


	

	//int id = curr_resource->GetResourceID(); 

	//switch (curr_resource->GetType())
	//{
	//case RESOURCE_TYPE_MESH:
	//	m
	//	break; 
	//}

	return curr_resource; 
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

			
			meshes_loader->LoadFileScene(path);
			
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
			ComponentMaterial* mat = material_loader->ImportImage(path);

			if (mat != nullptr)
				App->scene_intro->GetCurrentGO()->PushComponent((Component*)mat); 
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

Resource::Resource(const char* name, resource_t type, Component* resource_cmp)
{
	this->name = name; 
	this->type = type; 
	this->resource_data = resource_cmp; 
	this->id = App->resource_manager->curr_id; 
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

int Resource::GetResourceID() const
{
	return id;
}

void Resource::SetResourceID(int id)
{
	this->id = id; 
}

resource_t Resource::GetType() const
{
	return type;
}

Component * Resource::GetData() const
{
	return resource_data;
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


