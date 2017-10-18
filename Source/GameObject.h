#pragma once

#ifndef _GIZMO_H_
#define _GIZMO_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "OpenGL.h"
#include <list>

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
	component_type type; 

	virtual bool Enable(); 
	virtual bool Update();
	virtual bool Disable();

};

struct ComponentMeshRenderer : public Component
{
	float* vertices;
	int num_vertices = 0;

	uint* indices;
	int num_indices = 0;

	float* uvs;
	int num_uvs = 0;

	uint vertices_id = 0;
	uint uvs_id = 0;
	uint indices_id = 0;

	AABB Gizmo_box;


	void SetCubeVertices(float3 origin, uint edge_size); 
	void SetSphereVertices(float radius, uint rings, uint sectors, float3 origin);
	void SetCylinderVertices(float r, int sides, int height);
	uint GetTriNum() const;

	void SetGizmoBox(AABB _box);
	AABB GetGizmoBox() const;
};

struct ComponentMaterial : public Component
{

	void SetTextureID(GLuint tex); 
	uint GetTexSize()const;

	uint textures_id = 0;

	uint width = 0; 
	uint height = 0;

	std::string path;  
};
struct ComponentTransform : public Component
{
public: 
	void SetPosition(const float3& position);
	void SetRotation(const Quat& rotation);
	void SetScale(const float3& scale);

	float3 GetPosition() const;
	Quat GetRotation() const;
	float3 GetScale() const;

private: 
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;
};

class GameObject
{
public:

	GameObject() {}
	~GameObject() {}

	virtual void Draw(); 

	bool Active()const;
	void SetActive(bool _active);

	//uint GetBufferNum();
	
	Component* GetComponent(component_type new_component);
	void PushComponent(Component* comp); 

private:
	vector<Component*> component_list;
	vector<GameObject*> child_list;
	GameObject* parent; 

	bool active = false;

};

//class Cube1 : public Gizmo
//{
//public:
//	Cube1();
//	~Cube1(); 
//
//	void Start();
//
//	void Draw(); 
//};
//
//class Cube2 : public Gizmo
//{
//public:
//	Cube2();
//	~Cube2();
//
//	void Start(float3 origin, float size);
//
//	void Draw();
//};
//
//class GLSphere : public Gizmo
//{
//public: 
//	GLSphere();
//	~GLSphere();
//
//	void Start(float radius, uint rings, uint sectors, float3 origin);
//	void Draw(); 
//
//protected:
//
//	std::vector<GLfloat> vertices;
//	std::vector<GLfloat> normals;
//	std::vector<GLushort> indices;
//};
//
//class GLCylinder : public Gizmo
//{
//public:
//	GLCylinder();
//	~GLCylinder();
//
//	void Start(float r, int sides, int lenght);
//	void Draw();
//
//protected:
//	std::vector<float3> vertices;
//	std::vector<GLushort> indices;
//
////};
//class GLGizmo : public Gizmo
//{
//public:
//	GLGizmo();
//	~GLGizmo();
//	
//	void Start(float r, int sides, int lenght);
//	void Draw();
//	void SetGizmoBox(AABB _box);
//	
//
//		
//public:
//					
//	std::list<MeshRenderer> mr_list; 
//	Material material; 
//	std::string path_name = "GameObject";
//	AABB Gizmo_box;
//
//};

#endif // !_ELEMENT_H_

