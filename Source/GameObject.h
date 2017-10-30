#pragma once

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "OpenGL.h"
#include <list>

class Component; 
enum component_type; 
enum mesh_shape; 

class GameObject
{
public:

	GameObject();
	~GameObject() {}

	void Draw(); 

	bool IsActive()const;
	void SetActive(bool _active);

	bool IsStatic()const;
	void SetStatic(bool _active);

	void SetName(const char* name);
	const char* GetName(); 
	void SetID(uint id);
	uint GetID(); 

	Component* GetComponent(component_type new_component, int skip_num = 0);
	Component* GetComponent(int index); 

	GameObject* GetChild(uint id); 

	bool IsCompEmpty(); 
	bool IsChildEmpty(); 

	uint GetNumChilds(); 
	uint GetNumComponents();
	GameObject* GetParent(); 

	void PushComponent(Component* comp); 
	void PushChild(GameObject* child); 

private:
	vector<Component*> component_list;
	vector<GameObject*> child_list;

	GameObject* parent; 
	uint id;	
	string name = "GameObject"; 

	bool active = false;
	bool is_static = false; 

};

#endif

