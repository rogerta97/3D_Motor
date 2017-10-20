#ifndef _COMPONENTMATERIAL_H_
#define _COMPONENTMATERIAL_H_

#include "Globals.h"
#include "OpenGL.h"
#include <string>
#include "Component.h"

class ComponentMaterial : public Component
{
public: 
	ComponentMaterial();

	~ComponentMaterial();

	bool Enable();
	bool Update();
	bool Disable();

	void Set(ComponentMaterial* new_cmp);

	void SetTextureID(GLuint tex);
	uint GetTexSize()const;

	uint textures_id = 0;

	uint width = 0;
	uint height = 0;

	std::string path;
};

#endif
