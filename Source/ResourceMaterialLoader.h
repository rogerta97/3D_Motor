#ifndef _MATERIALS_IMPORTER_H_
#define _MATERIALS_IMPORTER_H_

#include "Globals.h"
#include "ResourceManager.h"
#include <list>

class ComponentMaterial;

class ResourceMaterialLoader
{
public:
	ResourceMaterialLoader();
	~ResourceMaterialLoader();

	bool Start();
	bool CleanUp();

	ComponentMaterial* ImportImage(const char* path);

	ComponentMaterial* GetComponentFromID(uint id);

	void SaveAsDDS();

	void RemoveMaterial(ComponentMaterial* mat);

public:
	std::map<uint, ComponentMaterial*> materials_loaded;

	uint save_id = 0;
};

#endif // !__MATERIALMANAGER__
