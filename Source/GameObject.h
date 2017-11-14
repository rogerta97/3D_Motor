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
	const char* GetName()const; 
	void SetID(uint id);
	uint GetID()const; 

	Component* GetComponent(component_type new_component, int skip_num = 0);
	Component* GetComponent(int index); 

	AABB GetBoundingBox(); 

	GameObject* GetChild(uint id)const; 
	bool IsChild(const GameObject* go) const;

	void AdaptBoundingBox(float4x4 transform); 

	bool IsCompEmpty(); 
	bool IsChildEmpty(); 

	bool IsRoot() const; 
	void SetRoot(const bool& _root); 

	uint GetNumChilds()const; 
	uint GetNumComponents()const;
	GameObject* GetParent()const; 

	void PushComponent(Component* comp); 
	Component* AddEmptyComponent(component_type type);
	void PushRootComponent(Component* comp); // In case of a root object selected the texture will need to be aded to all it's childs
	void PushChild(GameObject* child); 

public:
	vector<Component*> component_list;
	vector<GameObject*> child_list;

	GameObject* parent; 
	uint id = 0;	
	string name = "GameObject"; 

	bool active = false;
	bool is_static = false; 
	bool is_root = false; 

	UID UUID;

};

#endif

