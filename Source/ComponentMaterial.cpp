#include "ComponentMaterial.h"

void ComponentMaterial::Set(ComponentMaterial * new_cmp)
{
	textures_id = new_cmp->textures_id;
	width = new_cmp->width;
	height = new_cmp->height;
	path = new_cmp->path;
	type = new_cmp->type;
}

void ComponentMaterial::SetTextureID(GLuint tex)
{
	textures_id = tex;
}

uint ComponentMaterial::GetTexSize() const
{
	return 0;
}