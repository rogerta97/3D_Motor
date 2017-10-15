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
#include "Gizmo.h"

#include "mmgr/mmgr.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

class ModuleFBXLoader : public Module
{
public: 

	ModuleFBXLoader(bool enable_state = true); 
	~ModuleFBXLoader(); 
		
	bool Start();
	update_status Update(float dt); 
	bool CleanUp(); 

	void LoadFBX(const char* path);
	std::list<GLGizmo*>& GetList();
	GLGizmo* GetLastGizmo();
	void DrawElement(); 
	//sizes for the Bounding Box of a new gizmo
	float max_size = 0;
	float size = 0;

	GLuint ImportImage(const char* path); 

private:

	std::list<GLGizmo*> meshes;
	Material* info_material = nullptr;
	

};


#endif // !_FBX_LOADER_
