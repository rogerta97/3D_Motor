#include "ModuleFileSystem.h"
#include "Assimp/include/cfileio.h"
#include "PhysFS/include/physfs.h"
#include "Assimp/include/types.h"
#include "Application.h"

ModuleFileSystem::ModuleFileSystem(const char * game_path): Module(true)
{
	//init physfs
	char* root = SDL_GetBasePath();
	PHYSFS_init(root);
	
	AddPath(".");

	//CreateAssimpIO();
}

ModuleFileSystem::~ModuleFileSystem()
{
	//RELEASE(AssimpIO);
	
	//deinit physfs
	PHYSFS_deinit();
}

bool ModuleFileSystem::Init(json_file * config)
{
	LOG("Loading File System");
	bool ret = true;

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath(App->GetOrgName(), App->GetAppName());

	SDL_free(write_path);

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	return false;
}

bool ModuleFileSystem::AddPath(const char * path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG("Could not add path: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

bool ModuleFileSystem::Exists(const char * file) const
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFileSystem::IsDirectory(const char * file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

void ModuleFileSystem::DiscoverFiles(const char * directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const
{
}

bool ModuleFileSystem::CopyFromOutsideFS(const char * full_path, const char * destination)
{
	return false;
}

bool ModuleFileSystem::Copy(const char * source, const char * destination)
{
	return false;
}

void ModuleFileSystem::SplitFilePath(const char * full_path, std::string * path, std::string * file, std::string * extension) const
{
}

void ModuleFileSystem::NormalizePath(char * full_path) const
{
		int len = strlen(full_path);
		for (int i = 0; i < len; ++i)
		{
			if (full_path[i] == '\\')
				full_path[i] = '/';
			else
				full_path[i] = tolower(full_path[i]);
		}
	
}

