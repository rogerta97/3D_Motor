#ifndef __MODULEFILESYSTEM_H__
#define __MODULEFILESYSTEM_H__

#include "Module.h"
#include "Globals.h"

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(bool start_enabled = true);

	// Destructor
	~ModuleFileSystem();

	// Called before render is available
	bool Init(json_file* config);

	// Called before quitting
	bool CleanUp() ;

	// Utility functions	
	const char* GetFileName(const char* full_path)const;
	void NormalizePath(char* full_path) const;
	//Folder Functions
	string CreateFolder(const char* path, const char* name);
	void FileMove(const char* filepath, const char* new_path, bool replace_existing = true);
	void FileCopyPaste(const char* filepath, const char* new_path);
	void FileDelete(const char* filepath);
	bool SaveFile(const char * path, const char * file_content, const char * name, const char * extension, int size);
	
	//Gets
	string GetAssetsPath()const;
	string GetLibraryPath()const;

	// UTILITY
	vector<string> GetFilesInDirectory(const char* directory, const char* extension = "");
	void GetAllFilesInDirectory(const char* directory, vector<string>& to_fill);
	bool IsFolder(const char* directory);


public: 

		//Library
		string assets_path;
		string lib_path;
		string mesh_path;
		string tex_path;

		//Assets in Game
		string game_assets_dir; 
		string mesh_path_game;
		string tex_path_game;

};

#endif // __MODULEFILESYSTEM_H__
