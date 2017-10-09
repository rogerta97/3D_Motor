#pragma once

#ifndef _ELEMENT_H_
#define _ELEMENT_H_

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

protected: 

	uint buffer_num = 0; 

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

	void Start();

	void Draw();
};

class GLSphere : public Gizmo
{
public: 
	GLSphere();
	~GLSphere();

	void Start(float radius, uint rings, uint sectors);
	void Draw(); 

protected:
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> normals;
	std::vector<GLushort> indices;
};

#endif // !_ELEMENT_H_
