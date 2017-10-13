#pragma once
#ifndef  _TEXTURELOADER_H_
#define _TEXTURELOADER_H_

#define CHECKERS_HEIGHT 64
#define CHECKERS_WIDTH 64

#include "OpenGL.h"
#include "Globals.h"

#include "DevIL_Windows_SDK\include\IL\il.h"
#include "DevIL_Windows_SDK\include\IL\ilut.h"

#pragma comment (lib, "DevIL_Windows_SDK/libx86/DevIL.lib")
#pragma comment (lib, "DevIL_Windows_SDK/libx86/ILU.lib")
#pragma comment (lib, "DevIL_Windows_SDK/libx86/ILUT.lib")


class TextureLoader
{

public: 

	TextureLoader();
	~TextureLoader();

	bool Start(); 
	bool CleanUp(); 
	
	void LoadTextureBuffer(GLuint* texture_id);

	void LoadTestImage(); 
	
	GLubyte test_image[CHECKERS_HEIGHT][CHECKERS_WIDTH][4];

};

#endif // !_TEXTURELOADER_H_

