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

#include "mmgr/mmgr.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct Mesh
{
	float* vertices;
	int num_vertices = 0;

	uint* indices;
	int num_indices = 0;

	int vertex_buffer; 
	int indices_buffer; 

	void Clean();
};

class ModuleFBXLoader : public Module
{
public: 

	ModuleFBXLoader(bool enable_state = true); 
	~ModuleFBXLoader(); 
		
	bool Start();
	update_status Update(float dt); 
	bool CleanUp(); 

	bool LoadFBX(const char* path);
	void DrawElement(); 
	
private:

	// Current FBX info
	Mesh curr_mesh; 

};


#endif // !_FBX_LOADER_
