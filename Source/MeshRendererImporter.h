#ifndef _MESH_RENDER_IMPORTER_
#define _MESH_RENDER_IMPORTER_

#include "Module.h"
#include <list>
#include <vector>

class ComponentMeshRenderer;
class ComponentMaterial;
class GameObject;
class aiNode;
class aiScene;
class aiMesh;

class MeshRendererImporter : public Module
{
public:
	MeshRendererImporter(bool enabled = true);
	~MeshRendererImporter();

	bool Init(json_file* config);
	bool Start();
	bool CleanUp();

	bool ImportFile(const char* path);

	void SaveToLibrary(ComponentMeshRenderer* mesh);
	void LoadFile(const char* path);

	void RemoveMesh(ComponentMeshRenderer* m);

private:
	bool RecursiveImportingChilds(const aiScene* scene, aiNode* node, GameObject* parent, const std::vector<ComponentMaterial*>& mats);
	bool ImportMesh(aiMesh* mesh, GameObject* owner, const std::vector<ComponentMaterial*>& mats);


private:
	std::list<ComponentMeshRenderer*> meshes;
	int id = 0;
	uint save_id = 0;

	ComponentMeshRenderer* cube = nullptr;

};

#endif // !_MESH_RENDER_IMPORTER_