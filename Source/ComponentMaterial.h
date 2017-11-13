#ifndef _COMPONENTMATERIAL_H_
#define _COMPONENTMATERIAL_H_

#include "Globals.h"
#include <string>
#include "Component.h"
#include "Color.h"


class ComponentMaterial : public Component
{
public: 
	ComponentMaterial(GameObject* parent);
	ComponentMaterial();

	~ComponentMaterial();

	bool Enable();
	bool Update();
	bool Disable();

	void OpenTexture(); 
	void CloseTexture(); 

	void Set(ComponentMaterial* new_cmp);

	void SetTextureID(uint tex);
	uint GetTexSize()const;

	uint textures_id = 0;

	uint width = 0;
	uint height = 0;

	std::string path;


};

#endif
