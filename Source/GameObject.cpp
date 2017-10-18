#include "GameObject.h"
#include "OpenGL.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "MathGeoLib\MathGeoLib.h"
#include <cmath>

#define M_PI 3.14
#define M_PI_2 M_PI/2

float3 ComponentTransform::GetPosition() const
{
	return position;
}

Quat ComponentTransform::GetRotation() const
{
	return rotation;
}

float3 ComponentTransform::GetScale() const
{
	return scale;
}

void ComponentTransform::SetPosition(const float3 & _position)
{
	position = _position;
}

void ComponentTransform::SetRotation(const Quat & rotation)
{
	this->rotation = rotation;
}

void ComponentTransform::SetScale(const float3 & scale)
{
	this->scale = scale;
}

void ComponentMeshRenderer::SetGizmoBox(AABB _box)
{
	Gizmo_box = _box;
}
AABB ComponentMeshRenderer::GetGizmoBox() const
{
	return Gizmo_box;
}
bool GameObject::Active() const
{
	return active;
}

void GameObject::SetActive(bool _active)
{
	active = _active;
}

uint ComponentMeshRenderer::GetTriNum() const
{
	return (num_vertices/3);
}

//Point ComponentMaterial::GetTexSize() const
//{
	//return texture_size;
//}

//Cube1::Cube1()
//{
//	
//}
//
//void Cube1::Start()
//{
//
//}
//
//Cube1::~Cube1()
//{
//}
//
//void Cube1::Draw()
//{
//	glEnableClientState(GL_VERTEX_ARRAY);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
//
//	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
//	glBindBuffer(GL_ARRAY_BUFFER, 0); 
//
//	glDisableClientState(GL_VERTEX_ARRAY); 
//}
//
//Cube2::Cube2()
//{
//}
//
//Cube2::~Cube2()
//{
//}
//
//void Cube2::Start(float3 origin, float size)
//{
//	
//
//	App->renderer3D->tex_loader.LoadTestImage(); 
//	App->renderer3D->tex_loader.LoadTextureBuffer(&textures_id);
//
//}
//
//void Cube2::Draw()
//{
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_TEXTURE_2D); 
//
//	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
//
//	GLubyte indices[] =
//	{			
//		0,4,2,
//		0,6,4, 
//		2,3,1,
//		0,2,1,
//		2,4,3,
//		4,5,3,
//		1,3,7,
//		7,3,5,
//		6,0,1,
//		6,1,7,
//		4,6,7,
//		4,7,5		
//	};
//
//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices); 
//	glBindTexture(GL_TEXTURE_2D, textures_id);
//
//	glDisableClientState(GL_VERTEX_ARRAY); 
//	glDisableClientState(GL_TEXTURE_2D);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0); 
//	glBindTexture(GL_TEXTURE_2D, 0);
//	
//}
//
//GLSphere::GLSphere()
//{	
//}
//
//GLSphere::~GLSphere()
//{
//}
//
//void GLSphere::Start(float radius, uint rings, uint sectors, float3 origin)
//{
//	translation = origin;
//	float const R = 1. / (float)(rings - 1);
//	float const S = 1. / (float)(sectors - 1);
//	int r, s;
//
//	vertices.resize(rings * sectors * 3);
//	normals.resize(rings * sectors * 3);
//
//	std::vector<GLfloat>::iterator v = vertices.begin();
//	std::vector<GLfloat>::iterator n = normals.begin();
//	for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
//		float const z = sin(-M_PI_2 + M_PI * r * R);
//		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
//		float const y = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
//
//		*v++ = x * radius;
//		*v++ = y * radius;
//		*v++ = z * radius;
//
//		*n++ = x;
//		*n++ = y;
//		*n++ = z;
//	}
//
//	indices.resize(rings * sectors * 4);
//	std::vector<GLushort>::iterator i = indices.begin();
//	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
//		*i++ = r * sectors + s;
//		*i++ = r * sectors + (s + 1);
//		*i++ = (r + 1) * sectors + (s + 1);
//		*i++ = (r + 1) * sectors + s;
//	}
//
//	glGenBuffers(1, (GLuint*)&vertices_id);
//
//	LOG("Sphere created with buffer num %d", vertices_id);
//	LOG("Radius: %.2f", radius);
//	LOG("Rings: %d", rings);
//	LOG("Sectors: %d", sectors);
//	LOG("Vertices: %d", (rings-2)*(sectors) + 2);
//
//}
//
//void GLSphere::Draw()
//{
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//
//	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
//	glNormalPointer(GL_FLOAT, 0, &normals[0]);
//	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_NORMAL_ARRAY);
//
//}
//
//GLCylinder::GLCylinder()
//{
//}
//
//GLCylinder::~GLCylinder()
//{
//}
//
//void GLCylinder::Start(float r, int sides, int height)
//{
//	vertices.resize(((360 / (360 / sides)) * 2) + 4);    //sides  (vertex per circumference) * 2 (circumferences) + 2 (circumference center)
//	std::vector<float3>::iterator v = vertices.begin();
//
//	*v++ = {0 ,  height*0.5f, 0 };     //vertices[0] is top center vertex
//	*v++ = {0 ,  -height*0.5f, 0 };    //vertices[1] is top bottom vertex
//
//	for (int a = 360; a >= 0; a -= (360 / sides))
//	{
//		float b = a * M_PI / 180; // degrees to radians
//		*v++ = { r * cos(b),height * 0.5f, r * sin(b) };    //vertices[2] is first vertex in top circumference
//		*v++ = { r * cos(b),-height*0.5f,  r * sin(b) };    //vertices[3] is first vertex in bottom circumference
//	}
//
//	indices.resize(vertices.size() * 6 - 4);
//	std::vector<GLushort>::iterator i = indices.begin();
//	for (int a = 2; (a + 2) < vertices.size(); a++)
//	{
//		*i++ = a + ((a + 1) % 2 ? 2 : 0); *i++ = a % 2; *i++ = a + (a % 2 ? 2 : 0);	// top and bottom circ
//		*i++ = a + ((a + 1) % 2 ? 0 : 2); *i++ = a + 1; *i++ = a + (a % 2 ? 0 : 2);   // side
//	}
//
//	glGenBuffers(1, (GLuint*)&vertices_id);
//	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, &vertices[0], GL_STATIC_DRAW);
//
//	LOG("Cylinder created with buffer num %d:", vertices_id);
//	LOG("Radius: %d:", r);
//	LOG("Sides: %d:", sides);
//	LOG("Height: %d:", height);
//	LOG("Vertices: %d", ((360 / (360 / sides)) * 2) + 2); 
//}
//
//void GLCylinder::Draw()
//{
//	glEnableClientState(GL_VERTEX_ARRAY);  
//	glBindBuffer(GL_ARRAY_BUFFER, vertices_id_t);
//	glVertexPointer(3, GL_FLOAT, 0, NULL); 
//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, 0); 
//}







void GameObject::Draw()
{
	if (!component_list.empty())
	{
		for (int i = 0; i < component_list.size(); i++)
		{
			if (component_list[i]->type == COMPONENT_MESH_RENDERER)
			{
				ComponentMeshRenderer* tmp = (ComponentMeshRenderer*)component_list[i];

				glEnableClientState(GL_VERTEX_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, tmp->vertices_id);
				glVertexPointer(3, GL_FLOAT, 0, NULL);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmp->indices_id);

				//Apply UV if exist
				if (tmp->num_uvs != 0)
				{
					glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					glBindBuffer(GL_ARRAY_BUFFER, tmp->uvs_id);
					glTexCoordPointer(3, GL_FLOAT, 0, NULL);
				}

				glDrawElements(GL_TRIANGLES, tmp->num_indices, GL_UNSIGNED_INT, NULL);

			}

			if (component_list[i]->type == COMPONENT_MATERIAL)
			{
				ComponentMaterial* tmp2 = (ComponentMaterial*)component_list[i];
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tmp2->textures_id);
		
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			//glBindTexture(GL_TEXTURE_2D, 0);
			
		}

	}

	
}

void ComponentMaterial::Set(ComponentMaterial * new_cmp)
{
	textures_id = new_cmp->textures_id; 
	width = new_cmp->width;
	height = new_cmp->height;
	path = new_cmp->path;
	type = new_cmp->type; 
}

void ComponentMaterial::SetTextureID(GLuint tex)
{
	textures_id = tex;
}

uint ComponentMaterial::GetTexSize() const
{
	return 0;
}

void ComponentMeshRenderer::SetCubeVertices(float3 origin, uint size)
{
	//translation = origin;
	glGenBuffers(1, (GLuint*)&vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);

	float vertices[8 * 3] =
	{
		origin.x - size / 2 ,origin.y + size / 2, origin.z - size / 2,
		origin.x - size / 2 ,origin.y - size / 2, origin.z - size / 2,
		origin.x + size / 2 ,origin.y + size / 2, origin.z - size / 2,
		origin.x + size / 2 ,origin.y - size / 2, origin.z - size / 2,
		origin.x + size / 2 ,origin.y + size / 2, origin.z + size / 2,
		origin.x + size / 2 ,origin.y - size / 2, origin.z + size / 2,
		origin.x - size / 2 ,origin.y + size / 2, origin.z + size / 2,
		origin.x - size / 2 ,origin.y - size / 2, origin.z + size / 2,
	};


	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, (GLuint*)&indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

	GLubyte indices[] =
	{			
		0,4,2,
		0,6,4, 
		2,3,1,
		0,2,1,
		2,4,3,
		4,5,3,
		1,3,7,
		7,3,5,
		6,0,1,
		6,1,7,
		4,6,7,
		4,7,5		
	};


	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 12 * 3, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	LOG("Cube created with buffer num %d", vertices_id);
	LOG("Vertices: 8");
	LOG("Triangles: 16");
}

void ComponentMeshRenderer::SetSphereVertices(float radius, uint rings, uint sectors, float3 origin)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const z = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const y = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*vertices++ = x * radius;
			*vertices++ = y * radius;
			*vertices++ = z * radius;


		}
	}
		

	for (r = 0; r < rings - 1; r++) for (s = 0; s < sectors - 1; s++) {
		*indices++ = r * sectors + s;
		*indices++ = r * sectors + (s + 1);
		*indices++ = (r + 1) * sectors + (s + 1);
		*indices++ = (r + 1) * sectors + s;
	}

	glGenBuffers(1, (GLuint*)&vertices_id);

	LOG("Sphere created with buffer num %d", vertices_id);
	LOG("Radius: %.2f", radius);
	LOG("Rings: %d", rings);
	LOG("Sectors: %d", sectors);
	LOG("Vertices: %d", (rings - 2)*(sectors)+2);
}

void ComponentMeshRenderer::SetCylinderVertices(float r, int sides, int height)
{
	//vertices.resize(((360 / (360 / sides)) * 2) + 4);    //sides  (vertex per circumference) * 2 (circumferences) + 2 (circumference center)
	//std::vector<float3>::iterator v = vertices.begin();

	//*v++ = {0 ,  height*0.5f, 0 };     //vertices[0] is top center vertex
	//*v++ = {0 ,  -height*0.5f, 0 };    //vertices[1] is top bottom vertex

	//for (int a = 360; a >= 0; a -= (360 / sides))
	//{
	//	float b = a * M_PI / 180; // degrees to radians
	//	*v++ = { r * cos(b),height * 0.5f, r * sin(b) };    //vertices[2] is first vertex in top circumference
	//	*v++ = { r * cos(b),-height*0.5f,  r * sin(b) };    //vertices[3] is first vertex in bottom circumference
	//}

	//indices.resize(vertices.size() * 6 - 4);
	//std::vector<GLushort>::iterator i = indices.begin();
	//for (int a = 2; (a + 2) < vertices.size(); a++)
	//{
	//	*i++ = a + ((a + 1) % 2 ? 2 : 0); *i++ = a % 2; *i++ = a + (a % 2 ? 2 : 0);	// top and bottom circ
	//	*i++ = a + ((a + 1) % 2 ? 0 : 2); *i++ = a + 1; *i++ = a + (a % 2 ? 0 : 2);   // side
	//}

	//glGenBuffers(1, (GLuint*)&vertices_id);
	//glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size()*3, &vertices[0], GL_STATIC_DRAW);

	//LOG("Cylinder created with buffer num %d:", vertices_id);
	//LOG("Radius: %d:", r);
	//LOG("Sides: %d:", sides);
	//LOG("Height: %d:", height);
	//LOG("Vertices: %d", ((360 / (360 / sides)) * 2) + 2); 
}

Component * GameObject::GetComponent(component_type new_component_type)
{
	//here we will create the component and add it to the list

	for (vector<Component*>::iterator it = component_list.begin();it != component_list.end(); it++)
	{
		if ((*it)->type == new_component_type)
			return (*it); 
	}

	return nullptr; 
	
}

void GameObject::PushComponent(Component * comp)
{
	component_list.push_back(comp); 
}

bool Component::Enable()
{ 
	return true;
}

bool Component::Update()
{
	return true;
}

bool Component::Disable()
{
	return true;
}
