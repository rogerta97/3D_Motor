#include "ComponentMeshRenderer.h"
#include "OpenGL.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

class ComponentTransform; 

void ComponentMeshRenderer::SetGizmoBox(AABB _box)
{
	bounding_box = _box;
}

AABB ComponentMeshRenderer::GetGizmoBox() const
{
	return bounding_box;
}

bool ComponentMeshRenderer::PrintBB()
{
	return false;
}

ComponentMeshRenderer::~ComponentMeshRenderer()
{

}

bool ComponentMeshRenderer::Enable()
{

	return true; 
}

bool ComponentMeshRenderer::Update()
{
	if (active == false)
		return true; 
	
	glEnableClientState(GL_VERTEX_ARRAY);

	ComponentTransform* ctransform = (ComponentTransform*)GetComponentParent()->GetComponent(COMPONENT_TRANSFORM);
	ComponentMaterial* cmaterial = (ComponentMaterial*)GetComponentParent()->GetComponent(COMPONENT_MATERIAL);
	
	if (App->renderer3D->curr_cam->selected_GO != nullptr)
	{
		if (!App->renderer3D->curr_cam->HasAABB(this->bounding_box)) return false;
	}
	glPushMatrix(); 
	glMultMatrixf(ctransform->GetGlobalTransform());

	//VERTICES
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (cmaterial != nullptr)
	{
		if (cmaterial->textures_id > 0)
		{
			glBindTexture(GL_TEXTURE_2D, cmaterial->textures_id);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//TEXTURE_COORDS
		if (cmaterial->textures_id > 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}
	}


	//INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indices_id);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// --

	//glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

	////Apply UV if exist
	//if (num_uvs != 0)
	//{
	//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//	glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
	//	glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//}

	////glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	return true;
}

bool ComponentMeshRenderer::Disable()
{
	active = false; 
	return true;
}

ComponentMeshRenderer::ComponentMeshRenderer()
{
	active = true;
	type = COMPONENT_MESH_RENDERER; 
}

uint ComponentMeshRenderer::GetTriNum() const
{
	return (num_vertices / 3);
}

void ComponentMeshRenderer::SetCubeVertices(float3 origin, uint size)
{
	//translation = origin;
	glGenBuffers(1, (GLuint*)&vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);

	float vertices_arr[8 * 3] =
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

	vertices = vertices_arr; 
	num_vertices = 8;

	float3 a = { origin.x - size / 2 ,origin.y - size / 2, origin.z - size / 2 }; 
	float3 b = { origin.x + size / 2 ,origin.y + size / 2, origin.z + size / 2 };

	bounding_box = AABB(a,b);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

	uint indices_arr[] =
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

	indices = indices_arr; 
	num_indices = 12 * 3; 

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 12 * 3, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	LOG("Cube created with buffer num %d", vertices_id);
	LOG("Vertices: 8");
	LOG("Triangles: 16");

	ComponentTransform* trans = (ComponentTransform*)parent->GetComponent(COMPONENT_TRANSFORM); 
	trans->SetPosition(origin); 
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