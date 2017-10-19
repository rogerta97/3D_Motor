#pragma once
#ifndef _IMPORTER_H_
#define _IMPORTER_H_

#include "Globals.h"
#include "GameObject.h"
#include "Module.h"
#include "Application.h"


class MaterialImporter : public Module
{
public:
	MaterialImporter();
	~MaterialImporter();
	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file, SDL_Texture* resource);
	bool LoadCheckers(SDL_Texture* resource);
}; 
#endif // !1
