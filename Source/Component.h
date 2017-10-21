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

enum mesh_shape
{
	EMPTY,
	CUBE,
};

class Component
{
public:

	Component() {}

	component_type type;

	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();
	bool IsActive()const;

	GameObject* GetComponentParent() { return parent; }
	void SetComponentParent(GameObject* parent); 

	~Component() {}

protected: 

	bool active = false;
	GameObject* parent; 

};

#endif
