#include "OpenGL.h"
#include "ComponentMaterial.h"


ComponentMaterial::ComponentMaterial(GameObject* _parent)
{
	parent = _parent; 
	type = COMPONENT_MATERIAL; 
}

ComponentMaterial::ComponentMaterial()
{
	type = COMPONENT_MATERIAL;
}

ComponentMaterial::~ComponentMaterial()
{
}

bool ComponentMaterial::Enable()
{
	active = true; 

	return true;
}

bool ComponentMaterial::Update()
{

	if (active == false)
		return true;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures_id);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0); 

	return true;
}

bool ComponentMaterial::Disable()
{
	active = false; 

	return true;
}

void ComponentMaterial::DeleteComponent()
{
}

void ComponentMaterial::OnLoad(json_file * config)
{
	//what should we Save/Load???
}

void ComponentMaterial::OnSave(json_file & config) const
{
	//what should we Save/Load???
}

void ComponentMaterial::OpenTexture()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures_id);
}

void ComponentMaterial::CloseTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ComponentMaterial::Set(ComponentMaterial * new_cmp)
{
	textures_id = new_cmp->textures_id;
	width = new_cmp->width;
	height = new_cmp->height;
	path = new_cmp->path;
	type = new_cmp->type;
}

void ComponentMaterial::SetTextureID(uint tex)
{
	textures_id = tex;
}

uint ComponentMaterial::GetTexSize() const
{
	return 0;
}
//
//Color* ComponentMaterial::GetColor()
//{
//	return color;
//}

