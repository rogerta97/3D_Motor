#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include "Globals.h"


class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(const char* game_path = nullptr);

	// Destructor
	~ModuleFileSystem();

	// Called before render is available
	bool Init(json_file* config);

	// Called before quitting
	bool CleanUp() ;

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const;
	bool CopyFromOutsideFS(const char* full_path, const char* destination);
	bool Copy(const char* source, const char* destination);
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr) const;
	void NormalizePath(char* full_path) const;

	const char* GetFileName(const char* full_path)
	{
		std::string full_path_str(full_path);
		uint cut1 = full_path_str.find_last_of('\\');
		uint cut2 = full_path_str.find_last_of('.');

		uint lenght = cut2 - cut1;

		std::string final_str = full_path_str.substr(cut1 + 1, cut2 - 1);
	
		return final_str.c_str(); 
	}

	// Open for Read/Write
	//unsigned int Load(const char* path, const char* file, char** buffer) const;
	//unsigned int Load(const char* file, char** buffer) const;
	//SDL_RWops* Load(const char* file) const;
	//void* BassLoad(const char* file) const;

	//// IO interfaces for other libs to handle files via PHYSfs
	//aiFileIO* GetAssimpIO();

	//unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false) const;
	//bool Remove(const char* file);

	//const char* GetBasePath() const;
	//const char* GetWritePath() const;
	//const char* GetReadPaths() const;

private:

	void CreateAssimpIO();

private:

	//aiFileIO* AssimpIO = nullptr;
};

#endif // __MODULEFILESYSTEM_H__
