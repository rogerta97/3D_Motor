#ifndef _FBX_LOADER_
#define _FBX_LOADER_

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "Module.h"
#include <list>
#include "Globals.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "mmgr/mmgr.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

class ComponentMaterial; 
class ComponentMeshRenderer;

class ResourceMeshLoader
{
public: 

	ResourceMeshLoader();
	~ResourceMeshLoader();
		
	bool Start();
	update_status Update(float dt); 
	bool CleanUp(); 

	void LoadFBX(const char* path, aiNode* node, const aiScene* scene, GameObject* parent);
	void LoadFileScene(const char* path); 
	void GetInnerTransform(aiNode& node, aiVector3D& pos, aiQuaternion& quat, aiVector3D& scale);

	void SaveToLibrary(ComponentMeshRenderer* mesh);
	void DrawElement(); 

	Resource* SetResource(std::string name, resource_t type, Component* resource_cmp);



public:
	//variable to incresease when saving files
	uint lib_id = 0;

	//sizes for the Bounding Box of a new gizmo
	float max_size = 0;
	float size = 0;

	vector<ComponentMeshRenderer*> meshes_loaded; 	
};


#endif // !_FBX_LOADER_
