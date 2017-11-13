#include "ModuleMaterials.h"
#include "DevIL_Windows_SDK\include\IL\il.h"
#include "DevIL_Windows_SDK\include\IL\ilu.h"
#include "DevIL_Windows_SDK\include\IL\ilut.h"
#include "JSON.h"


#include "mmgr/mmgr.h"
#pragma comment (lib,"DevIL_Windows_SDK/libx86/DevIL.lib")
#pragma comment( lib, "DevIL_Windows_SDK/libx86/ILU.lib" )
#pragma comment( lib, "DevIL_Windows_SDK/libx86/ILUT.lib" )

MaterialImporter::MaterialImporter()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

MaterialImporter::~MaterialImporter()
{
	ilShutDown();
}

bool MaterialImporter::Import(const char * file, const char * path, std::string & output_file)
{
	bool ret = false;
	
	ILuint size;
	ILubyte *data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer
	if (size > 0) {
		data = new ILubyte[size]; // allocate data buffer
		//if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			//create the folder on the 
		//	ret = App->json->SaveUnique(output_file, data, size, "/Library/Materials/", "texture", "dds");
		RELEASE_ARRAY(data);
	}
	return false;
}

bool MaterialImporter::Import(const void * buffer, uint size, std::string & output_file)
{

	return false;
}

bool MaterialImporter::Load(const char * exported_file, SDL_Texture * resource)
{
	
	return false;
}
