#include "GameObject.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include <cmath>
#include "ComponentDefs.h"

bool GameObject::IsActive() const
{
	return active;
}

void GameObject::SetActive(bool _active)
{
	active = _active;
}

bool GameObject::IsStatic() const
{
	return is_static;
}

void GameObject::SetStatic(bool _active)
{
	is_static = _active; 
}

void GameObject::SetName(const char * _name)
{
	name = _name; 
}

const char * GameObject::GetName()
{
	return name.c_str();
}

void GameObject::SetID(uint _id)
{
	id = _id;
}

uint GameObject::GetID()
{
	return id;
}

GameObject::GameObject()
{

	parent = nullptr;
	active = true; 
	is_static = false; 

	// This will be removed when good FBX loading is implemented

	id = App->scene_intro->GetList().size(); 
	name += " ";
	name += to_string(id + 1);

	// ---------------------

	ComponentTransform* trans = new ComponentTransform(this);
	PushComponent(trans);

	trans->SetIdentityTransform();
	trans->type = COMPONENT_TRANSFORM;


}

void GameObject::Draw()
{
	if (!active)
		return; 

	if (!component_list.empty())
	{
		for (int i = 0; i < component_list.size(); i++)
		{
			if (component_list[i]->IsActive() == false)
				return;

			component_list[i]->Update();
		}
	}	

	if (!child_list.empty())
	{
		for (int i = 0; i < child_list.size(); i++)
		{
			child_list[i]->Draw();
		}
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

Component * GameObject::GetComponent(int index)
{
	int count = 0; 
	for (std::vector<Component*>::iterator it = component_list.begin(); it != component_list.end(); it++)
	{
		if (count++ == index)
			return (*it); 
	}
}

GameObject * GameObject::GetChild(uint id)
{
	return child_list.at(id);
}

bool GameObject::IsCompEmpty()
{
	return component_list.empty();
}

bool GameObject::IsChildEmpty()
{
	return child_list.empty();
}

uint GameObject::GetNumChilds()
{
	return child_list.size();
}

uint GameObject::GetNumComponents()
{
	return component_list.size();
}

GameObject * GameObject::GetParent()
{
	return parent;
}

void GameObject::PushComponent(Component * comp)
{
	component_list.push_back(comp); 	
}

void GameObject::PushChild(GameObject * child)
{
	child->parent = this;

	child_list.push_back(child); 
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
bool Component::SetActive(bool _active)
{
	active = _active;
	return active;
}
void Component::SetComponentParent(GameObject * _parent)
{
	parent = _parent; 
}

