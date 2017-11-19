#include "OpenGL.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "UID.h"
#include "JSON.h"

ComponentMaterial::ComponentMaterial(GameObject* _parent)
{
	parent = _parent; 
	type = COMPONENT_MATERIAL;


	void* a = this;
	void** a_ptr = &a;
	uint size = sizeof(this);
	char* data = new char[size];
	memcpy(data, a_ptr, size);

	uint* uid = md5(data, size);
	unique_id = *uid;

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

void ComponentMaterial::Delete()
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
	if (new_cmp != nullptr)
	{
		textures_id = new_cmp->textures_id;
		width = new_cmp->width;
		height = new_cmp->height;
		path = new_cmp->path;
		type = new_cmp->type;
	}
}

void ComponentMaterial::SetTextureID(uint tex)
{
	textures_id = tex;
}

uint ComponentMaterial::GetTexSize() const
{
	return 0;
}
void ComponentMaterial::Serialize(json_file * file)
{
	file->AddSectionToArray("Components");
	file->MoveToSectionFromArray("Components", file->GetArraySize("Components") - 1);

	file->SetInt("type", GetComponentType());
	file->SetInt("owner", GetComponentParent()->unique_id);
	file->SetInt("material", unique_id);
}
//
//Color* ComponentMaterial::GetColor()
//{
//	return color;
//}

