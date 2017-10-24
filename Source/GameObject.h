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

	virtual void Draw(); 

	bool Active()const;
	void SetActive(bool _active);

	void SetName(const char* name);
	const char* GetName(); 
	void SetNumMeshes(uint n_meshes);
	uint GetNumMeshes()const;

	Component* GetComponent(component_type new_component, int skip_num = 0);
	GameObject* GetChild(uint id); 

	bool IsCompEmpty(); 
	bool IsChildEmpty(); 

	uint GetNumChilds(); 
	uint GetNumComponents(); 

	void PushComponent(Component* comp); 
	void PushChild(GameObject* child); 

private:
	vector<Component*> component_list;
	vector<GameObject*> child_list;

	GameObject* parent; 
	uint num_meshes;
	bool active = false;
	string name = "GameObject"; 

};

#endif

