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

	list<string> GetFilesInDirectory(const char* directory); 


public: 
		string assets_path;
		string lib_path;
		string mesh_path;
		string tex_path;

};

#endif // __MODULEFILESYSTEM_H__
