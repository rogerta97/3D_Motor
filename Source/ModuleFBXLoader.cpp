#include "ModuleFBXLoader.h"


ModuleFBXLoader::ModuleFBXLoader(bool enable_state)
{
}

ModuleFBXLoader::~ModuleFBXLoader()
{
}

bool ModuleFBXLoader::Start()
{

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

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

bool ModuleFBXLoader::LoadFBX(const char* full_path)
{
	bool ret = true; 

	if (curr_mesh.num_vertices != 0)
		curr_mesh.Clean(); 

	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[i];

			curr_mesh.num_vertices = new_mesh->mNumVertices;
			curr_mesh.vertices = new float[curr_mesh.num_vertices * 3];
			memcpy(curr_mesh.vertices, new_mesh->mVertices, sizeof(float) * curr_mesh.num_vertices * 3);
			LOG("New mesh with %d vertices", curr_mesh.num_vertices);

			glGenBuffers(1, (GLuint*)&curr_mesh.vertex_buffer); 
			glBindBuffer(GL_ARRAY_BUFFER, curr_mesh.vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*curr_mesh.num_vertices*3, curr_mesh.vertices, GL_STATIC_DRAW); 

			if (new_mesh->HasFaces())
			{
				curr_mesh.num_indices = new_mesh->mNumFaces * 3;
				curr_mesh.indices = new uint[curr_mesh.num_indices]; // assume each face is a triangle

				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
						
					else
						memcpy(&curr_mesh.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

				aiReleaseImport(scene);
			}

			glGenBuffers(1, (GLuint*)&curr_mesh.indices_buffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr_mesh.indices_buffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*curr_mesh.num_indices, curr_mesh.indices, GL_STATIC_DRAW);
		}
	}
	else
	{
		LOG("Error loading scene %s", full_path);
		ret = false; 
	}
		

	return ret; 
}

void ModuleFBXLoader::DrawElement()
{
	glEnableClientState(GL_VERTEX_ARRAY); 

	glBindBuffer(GL_ARRAY_BUFFER, curr_mesh.vertex_buffer); 

	glVertexPointer(3, GL_FLOAT, 0, NULL); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr_mesh.indices_buffer);

	glDrawElements(GL_TRIANGLES, curr_mesh.num_indices, GL_UNSIGNED_INT, NULL); 

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Clean()
{
	//delete(vertices);
	//vertices = nullptr; 

	//delete(indices); 
//	indices = nullptr;
}
