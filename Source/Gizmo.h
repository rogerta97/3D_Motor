#pragma once

#ifndef _GIZMO_H_
#define _GIZMO_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "OpenGL.h"
#include <list>

struct MeshRenderer
{
	float* vertices;
	int num_vertices = 0;

	uint* indices;
	int num_indices = 0;

	float* uvs;
	int num_uvs = 0;

	uint vertices_id_t = 0;
	uint uvs_id_t = 0;
	uint indices_id_t = 0;
};

struct Material
{

	void SetTextureID(GLuint tex); 

	uint textures_id_t = 0;

	uint width = 0; 
	uint height = 0;

	std::string path;  
};

class Gizmo
{
public:

	Gizmo() {}
	~Gizmo() {}

	virtual void Start();
	virtual void Draw(); 
	float3 GetPosition() const;
	float3 GetRotation() const;
	float3 GetScale() const;

	void SetPosition(const float3& position);
	void SetRotation(const float3& rotation);
	void SetScale(const float3& scale);
	bool Active()const;
	void SetActive();

	//uint GetBufferNum() ;
	uint GetTriNum() const;
	uint GetTexSize()const;

protected: 
	float3 translation = float3::zero;
	float3 rotation = float3::one;
	float3 scale = float3::one;

	uint triangles_num = 0;
	uint texture_size;//? size or dimensions

	bool active = false;
public:

	uint vertices_id = 0;
	uint uvs_id = 0;
	uint textures_id = 0;
	uint indices_id = 0;

};

class Cube1 : public Gizmo
{
public:
	Cube1();
	~Cube1(); 

	void Start();

	void Draw(); 
};

class Cube2 : public Gizmo
{
public:
	Cube2();
	~Cube2();

	void Start(float3 origin, float size);

	void Draw();
};

class GLSphere : public Gizmo
{
public: 
	GLSphere();
	~GLSphere();

	void Start(float radius, uint rings, uint sectors, float3 origin);
	void Draw(); 

protected:

	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLushort> indices;
};

class GLCylinder : public Gizmo
{
public:
	GLCylinder();
	~GLCylinder();

	void Start(float r, int sides, int lenght);
	void Draw();

protected:
	std::vector<float3> vertices;
	std::vector<GLushort> indices;

};
class GLGizmo : public Gizmo
{
public:
	GLGizmo();
	~GLGizmo();
	
		void Start(float r, int sides, int lenght);
	void Draw();
	void SetGizmoBox(AABB _box);
	AABB GetGizmoBox() const;

		
public:
					
	std::list<MeshRenderer> mr_list; 
	Material material; 

	AABB Gizmo_box;

};

#endif // !_ELEMENT_H_

