#include "ModuleFBXLoader.h"


ModuleFBXLoader::ModuleFBXLoader()
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

	return update_status();
}

bool ModuleFBXLoader::CleanUp()
{

	aiDetachAllLogStreams(); 

	return true;
}

bool ModuleFBXLoader::LoadFBX(const char* full_path)
{
	bool ret = true; 

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

			if (new_mesh->HasFaces())
			{
				curr_mesh.num_indices = new_mesh->mNumFaces * 3;
				curr_mesh.indices = new uint[curr_mesh.num_indices]; // assume each face is a triangle

				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						LOG("WARNING, geometry face with != 3 indices!");

					else
						memcpy(&curr_mesh.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

				aiReleaseImport(scene);
			}
		}
	}
	else
	{
		LOG("Error loading scene %s", full_path);
		ret = false; 
	}
		

	return ret; 
}

