#pragma once

#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "OpenGL.h"
#include <list>

class Component; 
enum component_type; 

class GameObject
{
public:

	GameObject() {}
	~GameObject() {}

	virtual void Draw(); 

	bool Active()const;
	void SetActive(bool _active);

	void SetName(const char* name);
	const char* GetName(); 

	Component* GetComponent(component_type new_component);
	void PushComponent(Component* comp); 

private:
	vector<Component*> component_list;
	vector<GameObject*> child_list;
	GameObject* parent; 

	bool active = false;
	string name = "GameObject"; 

};

#endif

