#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class GameObject; 
class json_file;

enum component_type
{
	COMPONENT_MESH_RENDERER,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM,
	COMPONENT_CAMERA, 
	COMPONENT_NULL

};

enum mesh_shape
{
	EMPTY_MESH,
	CUBE_MESH,
	FBX_MESH
	
};

class Component
{
public:

	Component() {}

	component_type type;

	virtual bool Enable();
	virtual bool Update();
	virtual bool Disable();
	virtual void OnSave(json_file& config)const;
	virtual void OnLoad(json_file* config);
	bool IsActive()const;
	bool SetActive(bool _active);

	GameObject* GetComponentParent() { return parent; }
	void SetComponentParent(GameObject* parent); 

	~Component() {}

protected: 
	bool active = false;
	GameObject* parent; 

};

#endif
