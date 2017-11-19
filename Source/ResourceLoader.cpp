#include "Module.h"
#include "ResourceLoader.h"
#include "Application.h"
#include "JSON.h"
#include "GameObject.h"

ResourceLoader::ResourceLoader(bool start_enable) : Module(start_enable)
{
}

ResourceLoader::~ResourceLoader()
{
}

bool ResourceLoader::Start()
{
	return true;
}

update_status ResourceLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ResourceLoader::CleanUp()
{
	bool ret = true;

	for (std::map<uint, Resource*>::iterator it = resources.begin(); it != resources.end();)
	{
		RELEASE(it->second);

		it = resources.erase(it);
	}

	return ret;
}

bool ResourceLoader::Exists(std::string & file, int file_id)
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


