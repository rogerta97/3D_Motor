#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject; 

enum component_type
{
	COMPONENT_MESH_RENDERER,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM,
	COMPONENT_NULL

};

class Component
{
public:

	Component() {}

	component_type type;

	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();

	GameObject* GetComponentParent() { return parent; }

	~Component() {}

protected: 

	bool active = false;
	GameObject* parent; 

};

#endif
