
#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "OpenGL.h"
#include <list>

class Component; 
enum component_type; 
enum mesh_shape; 
class json_file;

class GameObject
{
	friend class Component;
public:

	GameObject();
	virtual ~GameObject() {}

	bool Save(json_file& config) const;
	void Load(json_file* config);
	void Draw(); 

	void Delete(); 

	// Status & info

	bool IsActive()const;
	void SetActive(bool _active);

	bool IsStatic()const;
	void SetStatic(bool _active);

	void SetName(const char* name);
	const char* GetName()const; 

	void SetID(uint id);
	uint GetID()const; 

	// Component

	Component* GetComponent(component_type new_component, int skip_num = 0);
	Component* GetComponent(int index); 
	void PushComponent(Component* comp);
	Component* AddEmptyComponent(component_type type);
	void PushRootComponent(Component* comp);
	uint GetNumComponents()const;
	void DeleteComponents(); 

	// Bounding Box 

	AABB GetBoundingBox(); 
	void RecursiveAdaptBoundingBox(float4x4 transform, GameObject* to_addapt);
	void AdaptBoundingBox(GameObject* to_addapt);

	// Parent & childs 

	GameObject* GetChild(uint id)const; 
	bool IsChild(const GameObject* go) const;
	uint GetNumChilds() const;
	void PushChild(GameObject* child);
	GameObject* AddChild();


	GameObject* GetParent() const;
	void SetParent(GameObject* parent);
	void DeleteParent();
	GameObject* GetSupreme(); 

	// Utility

	bool IsCompEmpty(); 
	bool IsChildEmpty(); 
	float DistanceTo(float3 point); 

	bool IsRoot() const; 
	void SetRoot(const bool& _root); 

	bool IsLooking4Parent();
	void SetLooking4Parent(bool is_searching);

	void ShowBB(bool show_it);



public:
	vector<Component*> component_list;
	vector<GameObject*> child_list;

	GameObject* parent; 
	string name = "GameObject"; 

	bool active = false;
	bool is_static = false; 
	bool is_root = false; 
	bool looking_for_parent = false; 

	UID unique_id;

};

#endif

