#ifndef _MATERIALS_IMPORTER_H_
#define _MATERIALS_IMPORTER_H_

#include "Globals.h"
#include "Module.h"
#include <list>

class ComponentMaterial;

class MaterialsImporter : public Module
{
public:
	MaterialsImporter(bool enabled = true);
	~MaterialsImporter();

	bool Init(json_file* config);
	bool Start();
	bool CleanUp();

	ComponentMaterial* ImportImage(const char* path);

	void SaveAsDDS();

	void RemoveMaterial(ComponentMaterial* mat);

private:
	std::list<ComponentMaterial*> materials;

	uint save_id = 0;
};

#endif // !__MATERIALMANAGER__
