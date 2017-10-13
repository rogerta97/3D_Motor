#pragma once

#ifndef _GIZMO_H_
#define _GIZMO_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "OpenGL.h"

class Gizmo
{
public:

	Gizmo() {}
	~Gizmo() {}

	virtual void Start();
	virtual void Draw(); 

public: 

	uint vertices_id = 0; 
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

public:

	float* vertices;
	int num_vertices = 0;

	uint* indices;
	int num_indices = 0;

	float* textures; 
	int num_textures = 0; 

};

#endif // !_ELEMENT_H_

