#include "ResourceMaterialLoader.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentMaterial.h"
#include "Functions.h"
#include "DevIL_Windows_SDK\include\IL\il.h"
#include "DevIL_Windows_SDK\include\IL\ilu.h"
#include "DevIL_Windows_SDK\include\IL\ilut.h"
#include <cstdio>


#pragma comment (lib, "DevIL.lib")
#pragma comment (lib, "ILU.lib")
#pragma comment (lib, "ILUT.lib")



ResourceMaterialLoader::ResourceMaterialLoader()
{
}

ResourceMaterialLoader::~ResourceMaterialLoader()
{
	ilShutDown();
}

bool ResourceMaterialLoader::Start()
{
	bool ret = true;

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	App->CreateFolder("Assets\\Materials");
	App->CreateFolder("Library\\Materials");

	return ret;
}

bool ResourceMaterialLoader::CleanUp()
{
	bool ret = true;
	//delete the non erased materials
	for (std::list<ComponentMaterial*>::iterator m = materials.begin(); m != materials.end(); )
	{
		RELEASE(*m);
		m = materials.erase(m);
	}

	return ret;
}

ComponentMaterial* ResourceMaterialLoader::ImportImage(const char* path)
{
	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ComponentMaterial* new_component = new ComponentMaterial(nullptr);
	
	success = ilLoadImage(path);

	if (success)
	{
		LOG("Loading new texture with path %s", path); 

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	
		new_component->width = ilGetInteger(IL_IMAGE_WIDTH);
		new_component->height = ilGetInteger(IL_IMAGE_HEIGHT);
		new_component->type = COMPONENT_MATERIAL;

		std::string full_path_str(path);
		uint cut1 = full_path_str.find_last_of('\\');

		std::string final_str = full_path_str.substr(cut1 + 1, full_path_str.size() - cut1);

		new_component->path = final_str; 
			
		if (!success)
		{
			LOG("Image conversion failed: %s\n", ilGetError());
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		new_component->textures_id = textureID; 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		SaveAsDDS();

	}
	else
	{
		textureID = 0;
		LOG("Not image found in %s", path); 
	}

	return new_component;


	//uint id = 0;
	//ILenum error;
	//ComponentMaterial* m = nullptr;
	////Data will be handled by renderer. Devil is only used to load the image.
	//if (ilLoad(IL_TYPE_UNKNOWN, path))
	//{
	//	ILinfo info;
	//	iluGetImageInfo(&info);
	//	if (info.Origin == IL_ORIGIN_UPPER_LEFT)
	//	{
	//		iluFlipImage();
	//	}

	//	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	//	int w = ilGetInteger(IL_IMAGE_WIDTH);
	//	int h = ilGetInteger(IL_IMAGE_HEIGHT);
	//	uint buff_id = 0;

	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//	glGenTextures(1, &buff_id);
	//	glBindTexture(GL_TEXTURE_2D, buff_id);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), w, h, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

	//	id = buff_id;

	//	m = new ComponentMaterial();
	//	materials.push_back(m);
	//	m->SetTextureID(id);
	//	m->width = w;
	//	m->height = h;
	//	m->path = path;

	//	SaveAsDDS();

	//	ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
	//}
	//else
	//{
	//	error = ilGetError();
	//	LOG("Error loading image %s. Error %d.", path, error);
	//}

	//return m;
}

void ResourceMaterialLoader::SaveAsDDS()
{
	ILuint		size;
	ILubyte*	data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			char file[69];
			sprintf_s(file, "Library\\Materials\\texture_%d.dds", save_id++);
			FILE* tex_file = fopen(file, "wb");
			fwrite(data, sizeof(ILubyte), size, tex_file);
			fclose(tex_file);
			LOG("New material texture saved: %s.", file);
		}
	}

}

void ResourceMaterialLoader::RemoveMaterial(ComponentMaterial * mat)
{
	for (std::list<ComponentMaterial*>::iterator m = materials.begin(); m != materials.end(); ++m)
	{
		if (*m == mat)
		{
			RELEASE(*m);

			materials.erase(m);
			break;
		}
	}
}
