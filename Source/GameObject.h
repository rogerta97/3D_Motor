
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
class ComponentTransform; 

class GameObject
{
	friend class Component;
public:

	GameObject();
	virtual ~GameObject() {}
	void Start();
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
	void DeleteComponent(component_type new_component);

	void PushRootComponent(Component* comp);
	uint GetNumComponents()const;
	void DeleteComponents(); 

	void GetChildByUID(uint UID, GameObject *& go) const;
	void EraseChild(GameObject * child, bool send_child_to_root);

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
	const std::vector<GameObject*> GetChilds() const;

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

	bool IsLooking4Reference();
	void SetLooking4Reference(bool is_searching);

	void ShowBB(bool show_it);

	void Serialize(json_file* file);
	void Serialize(json_file file);
	
	void SerializeLoad(json_file* file);
	void SerializeSave(json_file* file);

	void GetChildWithUID(uint UID, GameObject * go) const;

	uint GetNewChildID()const;
	void SetNewChildID(uint new_id);

	//For finding methods
	GameObject* Find(string name); 
	GameObject * CreateNewGO(std::string force_id);


public:
	vector<Component*> component_list;
	vector<GameObject*> child_list;

	GameObject* parent; 
	string name = "GameObject"; 

	bool active = false;
	bool is_static = false; 
	bool is_root = false; 

	bool looking_for_parent = false; 
	bool looking_for_reference = false; 

	uint new_child_id = 0;
	UID unique_id;

	ComponentTransform* transform; 

};

#endif

