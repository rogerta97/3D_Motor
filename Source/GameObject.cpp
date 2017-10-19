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

void GameObject::Draw()
{
	if (!component_list.empty())
	{
		for (int i = 0; i < component_list.size(); i++)
		{
			if (component_list[i]->type == COMPONENT_MESH_RENDERER)
			{
				ComponentMeshRenderer* tmp = (ComponentMeshRenderer*)component_list[i];

				glEnableClientState(GL_VERTEX_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, tmp->vertices_id);
				glVertexPointer(3, GL_FLOAT, 0, NULL);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmp->indices_id);

				//Apply UV if exist
				if (tmp->num_uvs != 0)
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, tmp->uvs_id);
					glTexCoordPointer(3, GL_FLOAT, 0, NULL);
				}

				glDrawElements(GL_TRIANGLES, tmp->num_indices, GL_UNSIGNED_INT, NULL);

			}

			if (component_list[i]->type == COMPONENT_MATERIAL)
			{
				ComponentMaterial* tmp2 = (ComponentMaterial*)component_list[i];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tmp2->textures_id);
		
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}		
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}

	
}

Component * GameObject::GetComponent(component_type new_component_type)
{
	//here we will create the component and add it to the list

	for (vector<Component*>::iterator it = component_list.begin();it != component_list.end(); it++)
	{
		if ((*it)->type == new_component_type)
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

