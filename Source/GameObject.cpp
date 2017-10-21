#include "GameObject.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include <cmath>
#include "ComponentDefs.h"

bool GameObject::Active() const
{
	return active;
}

void GameObject::SetActive(bool _active)
{
	active = _active;
}

void GameObject::SetName(const char * _name)
{
	name = _name; 
}

const char * GameObject::GetName()
{
	return name.c_str();
}

void GameObject::SetNumMeshes(uint n_meshes)
{
	num_meshes = n_meshes;
}

uint GameObject::GetNumMeshes() const
{
	return num_meshes;
}

GameObject::GameObject(mesh_shape shape)
{
	switch (shape)
	{
		case EMPTY_MESH: 
		{
			parent = nullptr;
			num_meshes = 0;
			SetActive(true);

			ComponentTransform* trans = new ComponentTransform();
			trans->SetIdentityTransform();
			trans->type = COMPONENT_TRANSFORM;
			PushComponent(trans);

			break;
		}

		case CUBE_MESH: 
		{
			ComponentMeshRenderer* cube_shape = new ComponentMeshRenderer();
			cube_shape->SetCubeVertices({ 0,0,0 }, 1);
			cube_shape->type = COMPONENT_MESH_RENDERER;
			PushComponent(cube_shape);

			parent = nullptr;
			SetActive(true);

			break;
		}

		case FBX_MESH:

			parent = nullptr;
			num_meshes = 0;
			SetActive(true);

			break; 

}
}

void GameObject::Draw()
{
	if (!component_list.empty())
	{
		for (int i = 0; i < component_list.size(); i++)
		{
			if (component_list[i]->IsActive() == false)
				return;
			component_list[i]->Update();//<----------
		}
				
		//glBindTexture(GL_TEXTURE_2D, 0);
	}	
}

Component * GameObject::GetComponent(component_type new_component_type, int skip_num)
{
	//here we will create the component and add it to the list

	int components_skipped = 0; 

	for (vector<Component*>::iterator it = component_list.begin();it != component_list.end(); it++)
	{
		if ((*it)->type == new_component_type)	
			components_skipped++; 
		
		if(components_skipped > skip_num)		
			return (*it); 
	}

	return nullptr; 
	
}

void GameObject::PushComponent(Component * comp)
{
	component_list.push_back(comp); 
}

bool Component::Enable()
{ 
	return true;
}

bool Component::Update()
{
	return true;
}

bool Component::Disable()
{
	return true;
}


bool Component::IsActive() const
{
	return active;
}
void Component::SetComponentParent(GameObject * _parent)
{
	parent = _parent; 
}

