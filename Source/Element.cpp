#include "Element.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include <cmath>

#define M_PI 3.14
#define M_PI_2 M_PI/2

void Gizmo::Start()
{
}

void Gizmo::Draw()
{
}

Cube1::Cube1()
{
	
}

void Cube1::Start()
{
	glGenBuffers(1, (GLuint*)&buffer_num);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_num);

	float3 origin(0, 0, 0);
	float vertices[36 * 3] =
	{
		origin.x + 1.0f, origin.y, origin.z,
		origin.x, origin.y, origin.z,
		origin.x, origin.y + 1, origin.z,

		origin.x, origin.y + 1, origin.z,
		origin.x + 1, origin.y + 1, origin.z,
		origin.x + 1, origin.y, origin.z,

		origin.x + 1, origin.y + 1, origin.z,
		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y, origin.z,

		origin.x + 1, origin.y, origin.z,
		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y, origin.z + 1,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x, origin.y + 1, origin.z + 1,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x, origin.y + 1, origin.z + 1,
		origin.x, origin.y, origin.z + 1,

		origin.x, origin.y + 1, origin.z + 1,
		origin.x, origin.y, origin.z,
		origin.x, origin.y, origin.z + 1,

		origin.x, origin.y + 1, origin.z + 1,
		origin.x, origin.y + 1, origin.z,
		origin.x, origin.y, origin.z,

		origin.x, origin.y + 1, origin.z,
		origin.x, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y + 1, origin.z + 1,

		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y + 1, origin.z,
		origin.x, origin.y + 1, origin.z,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x, origin.y, origin.z + 1,
		origin.x, origin.y, origin.z,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x, origin.y, origin.z,
		origin.x + 1, origin.y, origin.z,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Cube1::~Cube1()
{
}

void Cube1::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, buffer_num);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glDisableClientState(GL_VERTEX_ARRAY); 
}

Cube2::Cube2()
{
}

Cube2::~Cube2()
{
}

void Cube2::Start()
{
	glGenBuffers(1, (GLuint*)&buffer_num);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_num);

	float3 origin(5,5,5); 

	float vertices[8 * 3] =
	{
		15.f,5.f,5.f,
		10.f,5.f,5.f,
		10.f,0.f,5.f,
		15.f,0.f,5.f,
		15.f,0.f,0.f,
		15.f,5.f,0.f,
		10.f,5.f,0.f,
		10.f,0.f,0.f
	};


	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8*3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	App->renderer3D->tex_loader.LoadTestImage(); 

	App->renderer3D->tex_loader.LoadTextureBuffer(&tex_buffer_id);

}

void Cube2::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_2D);

	glBindBuffer(GL_ARRAY_BUFFER, buffer_num);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	GLubyte indices[] =
	{ 0,1,2, 2,3,0,
		0,3,4, 4,5,0,
		0,5,6, 6,1,0,
		1,6,7, 7,2,1,
		7,4,3, 3,2,7,
		4,7,6, 6,5,4 };

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices); 

	glDisableClientState(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY); 

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	
}

GLSphere::GLSphere()
{	
}

GLSphere::~GLSphere()
{
}

void GLSphere::Start(float radius, uint rings, uint sectors)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	vertices.resize(rings * sectors * 3);
	normals.resize(rings * sectors * 3);

	std::vector<GLfloat>::iterator v = vertices.begin();
	std::vector<GLfloat>::iterator n = normals.begin();
	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
		float const z = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const y = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		*v++ = x * radius;
		*v++ = y * radius;
		*v++ = z * radius;

		*n++ = x;
		*n++ = y;
		*n++ = z;
	}

	indices.resize(rings * sectors * 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*i++ = r * sectors + s;
		*i++ = r * sectors + (s + 1);
		*i++ = (r + 1) * sectors + (s + 1);
		*i++ = (r + 1) * sectors + s;
	}
}

void GLSphere::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}

GLCylinder::GLCylinder()
{
}

GLCylinder::~GLCylinder()
{
}

void GLCylinder::Start(float r, int sides, int height)
{
	vertices.resize(((360 / (360 / sides)) * 2) + 4);    //sides  (vertex per circumference) * 2 (circumferences) + 2 (circumference center)
	std::vector<float3>::iterator v = vertices.begin();

	*v++ = {0 ,  height*0.5f, 0 };     //vertices[0] is top center vertex
	*v++ = {0 ,  -height*0.5f, 0 };    //vertices[1] is top bottom vertex

	for (int a = 360; a >= 0; a -= (360 / sides))
	{
		float b = a * M_PI / 180; // degrees to radians
		*v++ = { r * cos(b),height * 0.5f, r * sin(b) };    //vertices[2] is first vertex in top circumference
		*v++ = { r * cos(b),-height*0.5f,  r * sin(b) };    //vertices[3] is first vertex in bottom circumference
	}

	indices.resize(vertices.size() * 6 - 4);
	std::vector<GLushort>::iterator i = indices.begin();
	for (int a = 2; (a + 2) < vertices.size(); a++)
	{
		*i++ = a + ((a + 1) % 2 ? 2 : 0); *i++ = a % 2; *i++ = a + (a % 2 ? 2 : 0);	// top and bottom circ
		*i++ = a + ((a + 1) % 2 ? 0 : 2); *i++ = a + 1; *i++ = a + (a % 2 ? 0 : 2);   // side
	}

	glGenBuffers(1, (GLuint*)&buffer_num);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_num);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, &vertices[0], GL_STATIC_DRAW);
}

void GLCylinder::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);  
	glBindBuffer(GL_ARRAY_BUFFER, buffer_num); 
	glVertexPointer(3, GL_FLOAT, 0, NULL); 
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
}
