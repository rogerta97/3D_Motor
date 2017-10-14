#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleCamera3D.h"
#include "Gizmo.h"
#include <vector>
ModuleFBXLoader::ModuleFBXLoader(bool enable_state)
{
}

ModuleFBXLoader::~ModuleFBXLoader()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

bool ModuleFBXLoader::Start()
{
	return true;
}

update_status ModuleFBXLoader::Update(float dt)
{

	return update_status::UPDATE_CONTINUE;
}

bool ModuleFBXLoader::CleanUp()
{

	aiDetachAllLogStreams(); 

	return true;
}

void ModuleFBXLoader::LoadFBX(const char* full_path)
{

	bool ret = true; 

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++) 
		{
			//Vertices
			aiMesh* m = scene->mMeshes[i];

			GLGizmo* new_mesh = new GLGizmo();

			new_mesh->num_vertices = m->mNumVertices;
			new_mesh->vertices = new float[new_mesh->num_vertices*3];
			memcpy(new_mesh->vertices, m->mVertices, sizeof(float) * new_mesh->num_vertices *3 );

			LOG("New mesh with %d vertices", new_mesh->num_vertices);
			glGenBuffers(1, (GLuint*) &new_mesh->vertices_id);
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->vertices_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			//Indices

			if (m->HasFaces()) {
				new_mesh->num_indices = m->mNumFaces * 3;
				new_mesh->indices = new uint[new_mesh->num_indices];

				for (uint i = 0; i < m->mNumFaces; ++i)
				{
					if (m->mFaces[i].mNumIndices != 3) {
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&new_mesh->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}

			glGenBuffers(1, (GLuint*) &new_mesh->indices_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->indices_id);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			if (m->HasTextureCoords(0)) // assume mesh has one texture coords
			{
				new_mesh->num_textures = m->mNumVertices;
				new_mesh->textures = new float[new_mesh->num_textures * 3];
				memcpy(new_mesh->textures, m->mTextureCoords[0], sizeof(float)*new_mesh->num_textures * 3);

				glGenBuffers(1, (GLuint*) &new_mesh->textures_id);
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_mesh->textures_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_textures * 3, new_mesh->textures, GL_STATIC_DRAW);
			}
			else
			{
				LOG("No Texture Coords found");
			}
			

			AABB bbox;
			bbox.SetNegativeInfinity();
			bbox.Enclose((float3*)m->mVertices, m->mNumVertices);
			new_mesh->SetGizmoBox(bbox);
			
			App->camera->Focus(vec3(new_mesh->GetPosition().x, new_mesh->GetPosition().y,new_mesh->GetPosition().z), bbox.Size().Length() *1.2f);


			meshes.push_back(new_mesh); 
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		aiReleaseImport(scene);
	
		
	}
	if (scene != nullptr && scene->HasMaterials())
	{
		aiMaterial* mat = scene->mMaterials[0]; //just one material is supported now
		aiString path;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		LOG("Mesh material path is: %s. Start importing it...", path.C_Str());

		
	}

	else
	{
		LOG("Error loading scene %s", full_path);
	}
		
}

std::list<GLGizmo*>& ModuleFBXLoader::GetList()
{
	return meshes; 
}

GLGizmo * ModuleFBXLoader::GetLastGizmo()
{
	if(meshes.empty() == false)
		return meshes.back();
	else
		return nullptr;
}

void ModuleFBXLoader::DrawElement()
{
	//glEnableClientState(GL_VERTEX_ARRAY); 

	//glBindBuffer(GL_ARRAY_BUFFER, curr_mesh.vertex_buffer); 

	//glVertexPointer(3, GL_FLOAT, 0, NULL); 

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr_mesh.indices_buffer);

	//glDrawElements(GL_TRIANGLES, curr_mesh.num_indices, GL_UNSIGNED_INT, NULL); 

	//glDisableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//void Mesh::Clean()
//{
//	//delete(vertices);
//	//vertices = nullptr; 
//
//	//delete(indices); 
////	indices = nullptr;
//}
