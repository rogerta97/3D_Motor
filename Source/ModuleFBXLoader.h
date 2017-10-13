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
	
private:

	std::list<GLGizmo*> meshes;

};


#endif // !_FBX_LOADER_
