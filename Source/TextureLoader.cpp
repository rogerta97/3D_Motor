#include "TextureLoader.h"

TextureLoader::TextureLoader()
{
}

TextureLoader::~TextureLoader()
{
	
}

bool TextureLoader::Start()
{




	
	return true;
}


bool TextureLoader::CleanUp()
{

	glDisable(GL_TEXTURE_2D); 
	return true;
}

void TextureLoader::LoadTextureBuffer(GLuint* texture_id)
{
	glGenTextures(1, texture_id);

	glBindTexture(GL_TEXTURE_2D, *texture_id);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, CHECKERS_WIDTH, CHECKERS_HEIGHT,
		0, GL_RGBA, GL_UNSIGNED_BYTE, test_image);

	glBindTexture(GL_TEXTURE_2D, 0);
}


void TextureLoader::LoadTestImage()
{
	for (int i = 0; i < CHECKERS_HEIGHT; i++)
	{
		for (int j = 0; j < CHECKERS_WIDTH; j++)
		{
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			test_image[i][j][0] = (GLubyte)c;
			test_image[i][j][1] = (GLubyte)c;
			test_image[i][j][2] = (GLubyte)c;
			test_image[i][j][3] = (GLubyte)255;
		}
	}
}
