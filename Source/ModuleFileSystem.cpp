#include "ModuleFileSystem.h"
#include "Application.h"
#include <Windows.h>

ModuleFileSystem::ModuleFileSystem(bool start_enabled): Module(start_enabled)
{
	//init physfs
	char* root = SDL_GetBasePath();

	assets_path = CreateFolder(root, "Assets");
	lib_path = CreateFolder(root, "Library");
	mesh_path = CreateFolder(lib_path.c_str(), "Meshes");
	tex_path = CreateFolder(lib_path.c_str(), "Textures");

	RELEASE(root);
}

ModuleFileSystem::~ModuleFileSystem()
{
	
	
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

string ModuleFileSystem::CreateFolder(const char * path, const char * name)
{
	string ret;

	string filepath = path;

	if (filepath[filepath.length() - 1] != '\\')
	{
		filepath += '\\';
	}

	filepath += name;

	DWORD error = GetLastError();

	if (CreateDirectory(filepath.c_str(), NULL) == 0)
	{
		error = GetLastError();
	}

	if (error == ERROR_PATH_NOT_FOUND)
	{
		LOG("Error creating folder (path not found): %s", path);
		return ret;
	}
	else if (error == ERROR_ALREADY_EXISTS)
	{
		LOG("Error creating folder (Folder aleady exists): %s", filepath.c_str())
	}

	ret = filepath + '\\';

	return ret;
}
void ModuleFileSystem::FileMove(const char * filepath, const char * new_path, bool replace_existing)
{
	string path = new_path;

	if (path[path.length() - 1] != '\\')
	{
		path += '\\';
	}

	path += GetFileName(filepath);

	if (!replace_existing)
	{
		if (MoveFile(filepath, path.c_str()))
		{
			LOG("Error moving file:[%s] to [%s]", filepath, path.c_str())
		}
	}
	else
	{
		if (MoveFileEx(filepath, path.c_str(), MOVEFILE_REPLACE_EXISTING))
		{
			LOG("Error moving file:[%s] to [%s]", filepath, path.c_str())
		}
	}
}

void ModuleFileSystem::FileCopyPaste(const char * filepath, const char * new_path)
{
	string path = new_path;

	if (path[path.length() - 1] != '\\')
	{
		path += '\\';
	}

	path += GetFileName(filepath);

	if (CopyFile(filepath, path.c_str(), false))
	{
		LOG("Error moving file:[%s] to [%s]", filepath, path.c_str())
	}

}

void ModuleFileSystem::FileDelete(const char * filepath)
{
	if (DeleteFile(filepath) == 0)
	{
		DWORD error = GetLastError();

		if (error == ERROR_FILE_NOT_FOUND)
		{
			LOG("Error deleting file (path not found)): %s", filepath);
		}
	}
}

bool ModuleFileSystem::SaveFile(const char * path, const char* file_content, const char* name, const char* extension, int size)
{
	bool ret = false;

	string file = path;
	file += name;
	file += ".";
	file += extension;

	std::ofstream;
	FILE* new_file = fopen(file.c_str(), "wb");

	if (new_file)
	{
		fwrite(file_content, sizeof(char), size, new_file);
		ret = true;
	}
	else
	{
		LOG("Error saving file %s: ", name);
	}

	fclose(new_file);

	return ret;
}
const char* ModuleFileSystem::GetFileName(const char* full_path)const
{
	std::string full_path_str(full_path);
	uint cut1 = full_path_str.find_last_of('\\');
	uint cut2 = full_path_str.find_last_of('.');

	uint lenght = cut2 - cut1;

	std::string final_str = full_path_str.substr(cut1 + 1, cut2 - 1);

	return final_str.c_str();
}

string ModuleFileSystem::GetAssetsPath() const
{
	return assets_path;
}

string ModuleFileSystem::GetLibraryPath() const
{
	return lib_path;
}
