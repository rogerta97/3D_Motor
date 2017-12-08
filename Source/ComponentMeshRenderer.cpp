#include "ComponentMeshRenderer.h"
#include "OpenGL.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "UID.h"
#include "JSON.h"

class ComponentTransform; 

void ComponentMeshRenderer::SetBBox(AABB _box)
{
	bounding_box = _box;

}

AABB ComponentMeshRenderer::GetBBox() const
{
	return bounding_box;
}

bool ComponentMeshRenderer::PrintBB()
{
	return false;
}

int ComponentMeshRenderer::GetNumVertices() const
{
	return num_vertices;
}

uint ComponentMeshRenderer::GetVerticesID() const
{
	return vertices_id;
}

vec * ComponentMeshRenderer::GetVertices() const
{
	return vertices;
}

int ComponentMeshRenderer::GetNumIndices() const
{
	return num_indices;
}

uint ComponentMeshRenderer::GetIndicesID() const
{
	return indices_id;
}

uint * ComponentMeshRenderer::GetIndices() const
{
	return indices;
}

int ComponentMeshRenderer::GetNumNormals() const
{
	return num_normals;
}

uint ComponentMeshRenderer::GetNormalsID() const
{
	return normals_id;
}

float3 * ComponentMeshRenderer::GetNormals() const
{
	return normals;
}

int ComponentMeshRenderer::GetNumUVS() const
{
	return num_uvs;
}

uint ComponentMeshRenderer::GetUVSID() const
{
	return uvs_id;
}

float * ComponentMeshRenderer::GetUVS() const
{
	return uvs;
}

void ComponentMeshRenderer::SetNumVertices(int num)
{
	num_vertices = num;
}

void ComponentMeshRenderer::SetVerticesId(uint id)
{
	vertices_id = id;
}

void ComponentMeshRenderer::SetVertices(vec * vec)
{
	vertices = vec;
}

void ComponentMeshRenderer::SetNumIndices(int num)
{
	num_indices = num;
}

void ComponentMeshRenderer::SetIndicesId(uint id)
{
	indices_id = id;
}

void ComponentMeshRenderer::SetIndices(uint * ind)
{
	indices = ind;
}

void ComponentMeshRenderer::SetNumNormals(int num)
{
	num_normals = num;
}

void ComponentMeshRenderer::SetNormalsId(uint id)
{
	normals_id = id; 
}

void ComponentMeshRenderer::SetNormals(float3 * ind)
{
	normals = ind;
}

void ComponentMeshRenderer::SetNumUVS(int num)
{
	num_uvs = num;
}

void ComponentMeshRenderer::SetUVSId(uint id)
{
	uvs_id = id;
}

void ComponentMeshRenderer::SetUVS(float * uvs)
{
	this->uvs = uvs;
}

bool ComponentMeshRenderer::IsBBoxShowing() const
{
	return show_bb;
	
}

bool ComponentMeshRenderer::AreNormalsShowing() const
{
	return show_normals;
}

void ComponentMeshRenderer::SetShowingAABB(bool show)
{
	show_bb = show; 
}

void ComponentMeshRenderer::SetShowingNormals(bool show)
{
	show_normals = show; 
}

void ComponentMeshRenderer::Serialize(json_file * file)
{
	file->AddSectionToArray("Components");
	file->MoveToSectionFromArray("Components", file->GetArraySize("Components") - 1);

	file->SetInt("type", GetComponentType());
	file->SetInt("owner", GetComponentParent()->unique_id);
	file->SetInt("mesh", unique_id);

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

	ComponentTransform* ctransform = (ComponentTransform*)gameobject->GetComponent(COMPONENT_TRANSFORM);
	ComponentMaterial* 	cmaterial = (ComponentMaterial*)gameobject->GetComponent(COMPONENT_MATERIAL);
		
	glPushMatrix(); 
	glMultMatrixf(ctransform->GetGlobalTransform().Transposed().ptr());	

	//VERTICES
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	bool transparent = false; 
	if (cmaterial != nullptr)
	{
		if (cmaterial->textures_id > 0)
		{
		
			glEnable(GL_BLEND);
			glEnable(GL_ALPHA_TEST);

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	glBindTexture(GL_TEXTURE_2D, 0); 

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glPopMatrix();

	//We modify the AABB if it is necessary 

	return true;
}

bool ComponentMeshRenderer::Disable()
{
	active = false; 
	return true;
}

void ComponentMeshRenderer::CleanUp()
{
}

void ComponentMeshRenderer::OnLoad(json_file * config)
{

}

void ComponentMeshRenderer::OnSave(json_file & config) const
{

}

void ComponentMeshRenderer::Delete()
{

	vertices = nullptr;
	glDeleteBuffers(1, &vertices_id); 
	delete(vertices);
	
	indices = nullptr;
	glDeleteBuffers(1, &indices_id);
	delete(indices); 

	normals = nullptr;
	glDeleteBuffers(1, &normals_id);
	delete (normals);

	uvs = nullptr;
	glDeleteBuffers(1, &uvs_id);
	delete (uvs); 

	delete(this); 

}

ComponentMeshRenderer::ComponentMeshRenderer(GameObject* _parent)
{
	active = true;
	gameobject = _parent; 
	show_bb = false; 
	show_normals = false; 
	type = COMPONENT_MESH_RENDERER; 

	//take the actual memory adress to not repeat the same
	void* a = this;
	void** a_ptr = &a;
	uint size = sizeof(this);
	char* data = new char[size];
	memcpy(data, a_ptr, size);

	uint* uid = md5(data, size);
	unique_id = *uid;
}

ComponentMeshRenderer::ComponentMeshRenderer(uint num_ver, float * ver, uint num_ind, uint * ind, uint num_uv, float * uv, uint num_norm, float * norm)
{
	//Load vertices to vram
	glGenBuffers(1, (GLuint*)&vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float) * num_vertices * 3, ver, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Load indices to vram
	glGenBuffers(1, (GLuint*)&indices_id);
	glBindBuffer(GL_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ARRAY_BUFFER,sizeof(uint) * num_indices, ind, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Load uv to vram
	if (uv != nullptr)
	{
		glGenBuffers(1, (GLuint*)&uvs_id);
		glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
		glBufferData(GL_ARRAY_BUFFER,sizeof(float) * num_uv * 3, uv, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	bounding_box.SetNegativeInfinity();
	bounding_box.Enclose((float3*)ver, num_ver);

	//take the actual memory adress to not repeat the same
	void* a = this;
	void** a_ptr = &a;
	uint size = sizeof(this);
	char* data = new char[size];
	memcpy(data, a_ptr, size);

	uint* uid = md5(data, size);
	unique_id = *uid;
}

uint ComponentMeshRenderer::GetNumTriangles() const
{
	return num_triangles;
}

void ComponentMeshRenderer::RecalculateCenterPoints()
{
	ComponentTransform* trans = (ComponentTransform*)GetComponentParent()->GetComponent(COMPONENT_TRANSFORM); 



	for (int i = 0; i < GetNumTriangles(); i++)
	{
		LineSegment segment(center_points[i], center_points[i] + normals[i]);
		segment.Transform(trans->GetGlobalTransform()); 

		center_points[i] = segment.a; 
		//normals[i] = segment.b; 
		//center_points[i] = trans->GetGlobalTransform(); 
	}
}

void ComponentMeshRenderer::SetNewMesh(ComponentMeshRenderer * new_mesh)
{
	if (new_mesh != nullptr)
	{
		vertices = new_mesh->vertices;
		num_vertices = new_mesh->num_vertices;
		vertices_id = new_mesh->vertices_id;

		indices = new_mesh->indices;
		num_indices = new_mesh->num_indices;
		indices_id = new_mesh->indices_id;

		uvs = new_mesh->uvs;
		num_uvs = new_mesh->num_uvs;
		uvs_id = new_mesh->uvs_id;

	}
}

void ComponentMeshRenderer::SetCubeVertices(float3 origin, uint size)
{
	//translation = origin;
	glGenBuffers(1, (GLuint*)&vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);

	num_vertices = 8;
	vertices = new vec[num_vertices];

	{
		vertices[0].x = origin.x - size / 2; 
		vertices[0].y = origin.y + size / 2;
		vertices[0].z = origin.z - size / 2;

		vertices[1].x = origin.x - size / 2;
		vertices[1].y = origin.y - size / 2;
		vertices[1].z = origin.z - size / 2;

		vertices[2].x = origin.x + size / 2;
		vertices[2].y = origin.y + size / 2;
		vertices[2].z = origin.z - size / 2;

		vertices[3].x = origin.x + size / 2;
		vertices[3].y = origin.y - size / 2;
		vertices[3].z = origin.z - size / 2;

		vertices[4].x = origin.x + size / 2;
		vertices[4].y = origin.y + size / 2;
		vertices[4].z = origin.z + size / 2;

		vertices[5].x = origin.x + size / 2;
		vertices[5].y = origin.y - size / 2;
		vertices[5].z = origin.z + size / 2;

		vertices[6].x = origin.x - size / 2;
		vertices[6].y = origin.y + size / 2;
		vertices[6].z = origin.z + size / 2;

		vertices[7].x = origin.x - size / 2;
		vertices[7].y = origin.y - size / 2;
		vertices[7].z = origin.z + size / 2;

	};


	//memcpy(vertices, &vertices_arr[0], sizeof(float) * 8 * 3);

	float3 a = { origin.x - size / 2 ,origin.y - size / 2, origin.z - size / 2 }; 
	float3 b = { origin.x + size / 2 ,origin.y + size / 2, origin.z + size / 2 };

	bounding_box = AABB(a,b);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

	num_indices = 36;
	num_triangles = num_indices / 3;
	indices = new uint[num_indices]; 

	indices[0] = 0;	indices[1] = 4;	indices[2] = 2;	indices[3] = 0;	indices[4] = 6;	indices[5] = 4,
	indices[6] = 2;	indices[7] = 3;	indices[8] = 1;	indices[9] = 0;	indices[10] = 2; indices[11] = 1;
	indices[12] = 2; indices[13] = 4; indices[14] = 3;	indices[15] = 4; indices[16] = 5; indices[17] = 3;
	indices[18] = 1; indices[19] = 3; indices[20] = 7;	indices[21] = 7; indices[22] = 3; indices[23] = 5;
	indices[24] = 6; indices[25] = 0; indices[26] = 1;	indices[27] = 6; indices[28] = 1; indices[29] = 7;
	indices[30] = 4; indices[31] = 6; indices[32] = 7;	indices[33] = 4; indices[34] = 7; indices[35] = 5;
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 12 * 3, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	LOG("Cube created with buffer num %d", vertices_id);
	LOG("Vertices: 8");
	LOG("Triangles: 16");
}

void ComponentMeshRenderer::SetPlaneVertices(float3 origin, uint edge_size, bool vertical)
{
	//translation = origin;
	glGenBuffers(1, (GLuint*)&vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);

	//Set the vertices 
	num_vertices = 4;
	vertices = new vec[num_vertices];

	if(vertical)
	{
		vertices[0].x = origin.x - edge_size / 2;
		vertices[0].y = origin.y + edge_size / 2;
		vertices[0].z = 0;

		vertices[1].x = origin.x - edge_size / 2;
		vertices[1].y = origin.y - edge_size / 2;
		vertices[1].z = 0;

		vertices[2].x = origin.x + edge_size / 2;
		vertices[2].y = origin.y - edge_size / 2;
		vertices[2].z = 0;
		
		vertices[3].x = origin.x + edge_size / 2;
		vertices[3].y = origin.y + edge_size / 2;
		vertices[3].z = 0;
	}
	else
	{
		vertices[0].x = origin.x - edge_size / 2;
		vertices[0].y = 0;
		vertices[0].z = origin.z - edge_size / 2;

		vertices[1].x = origin.x - edge_size / 2;
		vertices[1].y = 0;
		vertices[1].z = origin.z + edge_size / 2;

		vertices[2].x = origin.x + edge_size / 2;
		vertices[2].y = 0;
		vertices[2].z = origin.z + edge_size / 2;

		vertices[3].x = origin.x + edge_size / 2;
		vertices[3].y = 0;
		vertices[3].z = origin.z - edge_size / 2;
	}


	//memcpy(vertices, &vertices_arr[0], sizeof(float) * 8 * 3);

	float3 a = { origin.x - edge_size / 2 ,origin.y - edge_size / 2, origin.z - edge_size / 2 };
	float3 b = { origin.x + edge_size / 2 ,origin.y + edge_size / 2, origin.z + edge_size / 2 };

	bounding_box = AABB(a, b);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*)&indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

	// Set the indices
	num_indices = 6;
	num_triangles = num_indices / 3;

	indices = new uint[num_indices];
	center_points = new float3[num_triangles];

	num_normals = 2;
	normals = new float3[num_normals];

	indices[0] = 0;	
	indices[1] = 1;	
	indices[2] = 3;	
	indices[3] = 1;	
	indices[4] = 2;	
	indices[5] = 3;

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Normals 
	for (int i = 0; i < num_normals; i++)
	{
		Triangle current;
		current.a = vertices[indices[i * 3]];
		current.b = vertices[indices[i * 3 + 1]];
		current.c = vertices[indices[i * 3 + 2]];

		center_points[i] = current.CenterPoint();
		normals[i] = current.NormalCCW();
	}

	glGenBuffers(1, (GLuint*)&normals_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normals_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float3) * num_normals, normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Set UVs
	num_uvs = num_indices; 
	uvs = new float[num_vertices * 3];

	uvs[0] = 0;
	uvs[1] = 0; 

	uvs[2] = 1;
	uvs[3] = 0;

	uvs[4] = 1;
	uvs[5] = 1; 

	uvs[6] = 1;
	uvs[7] = 1;

	uvs[8] = 0;
	uvs[9] = 1;

	uvs[10] = 0;
	uvs[11] = 0; 

	glGenBuffers(1, (GLuint*)&uvs_id);
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_uvs * 2, uvs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	LOG("%d texture cordinates", num_uvs);

	LOG("Plane created with buffer num %d", vertices_id);
	LOG("Vertices: 4");
	LOG("Triangles: 2");
}

void ComponentMeshRenderer::SetSphereVertices(float radius, uint rings, uint sectors, float3 origin)
{
	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);
	int r, s;
	int count = 0; 

	for (r = 0; r < rings; r++)
	{
		for (s = 0; s < sectors; s++)
		{
			float const z = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const y = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			vertices[count].x = x * radius;
			vertices[count].y = y * radius;
			vertices[count].z = z * radius;

			count++; 
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