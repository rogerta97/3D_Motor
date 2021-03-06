#include "MeshRendererImporter.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "MaterialsImporter.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ComponentMaterial.h"
#include <direct.h>

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "AssimpLogger.h"
#include "Assimp\include\DefaultLogger.hpp"
#include "Assimp\include\Logger.hpp"

#pragma comment (lib, "Assimp/libx86/assimp.lib")



MeshRendererImporter::MeshRendererImporter(bool enabled): Module(enabled)
{
}

MeshRendererImporter::~MeshRendererImporter()
{
}

bool MeshRendererImporter::Init(json_file * config)
{
	bool ret = true;
	return ret;
}

bool MeshRendererImporter::Start()
{
	bool ret = true;

	Assimp::DefaultLogger::create();
	const uint severity = Assimp::Logger::Debugging | Assimp::Logger::Info | Assimp::Logger::Err | Assimp::Logger::Warn;
	
	Assimp::DefaultLogger::get()->attachStream(new AssimpLogger(), severity);


	//if we want something already loaded LoadFile() here
	return ret;
}

bool MeshRendererImporter::CleanUp()
{	bool ret = true;

	for (std::list<ComponentMeshRenderer*>::iterator g = meshes.begin(); g != meshes.end();)
	{
		(*g)->CleanUp();
		RELEASE(*g);

		g = meshes.erase(g);
	}

	aiDetachAllLogStreams();

	return ret;
}

bool MeshRendererImporter::ImportFile(const char * path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("%d Meshes detected. Start Loading...", scene->mNumMeshes);

		//Check if the scene has a material
		std::vector<ComponentMaterial*> mats;
		if (scene->HasMaterials())
		{
			for (uint m = 0; m < scene->mNumMaterials; ++m)
			{
				ComponentMaterial* material = nullptr;
				aiMaterial* curr_mat = scene->mMaterials[m];
				aiString assimp_path;
				curr_mat->GetTexture(aiTextureType_DIFFUSE, 0, &assimp_path);
				std::string path = assimp_path.C_Str();
				uint name_pos = path.find_last_of("\\");

				std::string rel_path = "Assets\\";
				rel_path += path.substr(name_pos + 1, path.size() - name_pos + 1);
				LOG("File material %d path is: %s. Start importing it...", m, rel_path.c_str());
				material = App->materials_importer->ImportImage(rel_path.c_str());
				mats.push_back(material);
			}
		}
		//Create base gameobject
		std::string file_path = path;
		uint name_start = file_path.find_last_of("\\");
		uint name_end = file_path.find_last_of(".");
		std::string root_go_name = file_path.substr(name_start + 1, name_end - name_start - 1);

		GameObject* root_go = App->scene_intro->CreateGameObject(root_go_name.c_str());

		//Import scene from root node
		ret = RecursiveImportingChilds(scene, scene->mRootNode, root_go, mats);
	}
	else
	{
		LOG("Error loading scene %s", path);
		ret = false;
	}

	return ret;
}

bool MeshRendererImporter::RecursiveImportingChilds(const aiScene * scene, aiNode * node, GameObject * parent, const std::vector<ComponentMaterial*>& mats)
{
	{
		//bool ret = true;
		//bool invalid_node = (node->mNumMeshes == 0);
		//bool invalid_mesh = false;

		//aiMesh* aimesh = nullptr;
		//ComponentMeshRenderer* c_mesh = nullptr;
		//ComponentTransform* c_transform = nullptr;
		//ComponentMaterial* c_material = nullptr;


		//GameObject* go = nullptr;
		////Get node transformation


		////Get node name
		//aiString name = node->mName;

		////Import node meshes
		//uint num_meshes = node->mNumMeshes;
		//GameObject* first_go = parent;
		//if (num_meshes > 0)
		//{
		//	for (uint i = 0; i < num_meshes; ++i)
		//	{
		//		GameObject* go = nullptr;
		//		//CHECK HERE IF MESH IS ALREADY LOADED!!!	

		//		int index = node->mMeshes[i];
		//		aimesh = scene->mMeshes[index];
		//		//Check if mesh is Valid
		//		if (!invalid_mesh && !invalid_node)
		//		{
		//			c_mesh = (ComponentMeshRenderer*)parent->AddEmptyComponent(COMPONENT_MESH_RENDERER);

		//			if (!aimesh->HasFaces())
		//			{
		//				invalid_mesh = true;
		//			}
		//		}
		//		//Set Transform
		//		float3 pos = float3::zero;
		//		Quat rot = Quat::identity;
		//		float3 scale = float3::zero;

		//		if (!invalid_node && !invalid_mesh)
		//		{
		//			aiVector3D translation;
		//			aiVector3D scaling;
		//			aiQuaternion rotation;
		//			node->mTransformation.Decompose(scaling, rotation, translation);
		//			pos = float3(translation.x, translation.y, translation.z);
		//			scale = float3(scaling.x, scaling.y, scaling.z);
		//			rot = Quat(rotation.x, rotation.y, rotation.z, rotation.w);

		//		
		//		}
		//		//Set vertices and indices

		//		if (!invalid_mesh && !invalid_node)
		//		{
		//			vec* vertices = new vec[aimesh->mNumVertices * 3];
		//			memcpy(vertices, aimesh->mVertices, sizeof(vec) * aimesh->mNumVertices * 3);

		//			uint* indices = new uint[aimesh->mNumFaces * 3];

		//			for (uint i = 0; i < aimesh->mNumFaces && !invalid_mesh; ++i)
		//			{
		//				if (aimesh->mFaces[i].mNumIndices == 3)
		//				{
		//					memcpy(&indices[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
		//				}
		//				else
		//					invalid_mesh = true;
		//			}
		//			c_mesh->SetIndices(indices);
		//			c_mesh->SetVertices(vertices);

		//			RELEASE_ARRAY(vertices);
		//			RELEASE_ARRAY(indices);
		//		}
		//		// UVS
		//		if (!invalid_mesh && !invalid_node && aimesh->HasTextureCoords(0))
		//		{
		//			float* uvs = new float[aimesh->mNumVertices * 3];
		//			memcpy(uvs, (float*)aimesh->mTextureCoords[0], sizeof(float) * aimesh->mNumVertices * 3);

		//			c_mesh->SetUVS(uvs);

		//			RELEASE_ARRAY(uvs);
		//		}
		//		//CreateGameObject
		//		if (!invalid_mesh && !invalid_node && parent != nullptr)
		//		{
		//			go = App->scene_intro->CreateGameObject("wtf");

		//			string name = node->mName.C_Str();
		//			if (name == "")
		//				name = "no_name";

		//			go->SetName(name.c_str());

		//			parent->PushChild(go);
		//			go->AddEmptyComponent(COMPONENT_TRANSFORM);
		//			ComponentTransform* c_trans = (ComponentTransform*)go->GetComponent(COMPONENT_TRANSFORM);
		//			c_trans->SetLocalPosition(pos);
		//			c_trans->SetLocalRotation(rot.ToEulerXYZ()*RADTODEG);
		//			c_trans->SetLocalScale(scale);

		//			go->AddEmptyComponent(COMPONENT_MESH_RENDERER);
		//			ComponentMeshRenderer* cmesh = (ComponentMeshRenderer*)go->GetComponent(COMPONENT_MESH_RENDERER);
		//			cmesh->SetNewMesh(c_mesh);

		//			if (c_material != nullptr)
		//			{
		//			go->AddEmptyComponent(COMPONENT_MATERIAL);
		//			ComponentMaterial* cmaterial = (ComponentMaterial*)go->GetComponent(COMPONENT_MATERIAL);
		//			cmaterial->Set(c_material);
		//			}
		//		}
		//		// Select parent
		//		GameObject* curr_parent = nullptr;
		//		if (!invalid_node && go != nullptr)
		//			curr_parent = go;
		//		else
		//			curr_parent = parent;

		//		//import mesh
		//		ret = ImportMesh(scene->mMeshes[node->mMeshes[i]], go, mats);
		//	}
		//}

	}
	bool ret = false;
	GameObject* root_go = parent;

	//Get node transformation
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	
	for (uint i = 0; i < node->mNumMeshes; ++i)
	{
		GameObject* go = nullptr;

		if (parent == nullptr)
		{
			go = App->scene_intro->CreateGameObject(node->mName.C_Str());
		}
		else
		{
			go = parent->AddChild();
			go->SetName(node->mName.C_Str());
		}

		if (i == 0)
			root_go = go;

		//import mesh
		ret = ImportMesh(scene->mMeshes[node->mMeshes[i]], go, mats);

		//set the transform to the go
		ComponentTransform* trans = (ComponentTransform*)go->GetComponent(COMPONENT_TRANSFORM);
		trans->SetLocalPosition(pos);
		trans->SetLocalRotation(rot.ToEulerXYZ() * RADTODEG);
		trans->SetLocalScale(scale);

	}

	//Get node name
	aiString name = node->mName;
	//Import child nodes
	for (uint i = 0; i < node->mNumChildren; i++)
	{
		RecursiveImportingChilds(scene, node->mChildren[i], root_go, mats);
	}

	return ret;
}

bool MeshRendererImporter::ImportMesh(aiMesh * mesh, GameObject * owner, const std::vector<ComponentMaterial*>& mats)
{
	bool ret = true;

	if (mesh == nullptr || owner == nullptr)
		ret = false;
	else
	{
		//Load mesh Vertices
		uint num_vertices = mesh->mNumVertices;
		float* vert = new float[num_vertices * 3];
		memcpy(vert, mesh->mVertices, sizeof(float) * num_vertices * 3);
		LOG("Mesh has %d vertices", num_vertices);

		//Load mesh Indices
		uint num_indices;
		uint* indices = nullptr;
		if (mesh->HasFaces())
		{
			num_indices = mesh->mNumFaces * 3;
			indices = new uint[num_indices]; // assume each face is a triangle
			for (uint i = 0; i < mesh->mNumFaces; ++i)
			{
				if (mesh->mFaces[i].mNumIndices != 3)
					ret = false;
				else
					memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
			LOG("Mesh has %d indices", num_indices);
		}

		//Load mesh UV
		uint num_uv = 0;
		float* uv = nullptr;
		if (mesh->HasTextureCoords(0)) // assume mesh has one texture coords
		{
			num_uv = mesh->mNumVertices;

			uv = new float[num_uv * 3];
			memcpy(uv, mesh->mTextureCoords[0], sizeof(float)*num_uv * 3);
		}
		else
		{
			LOG("No Texture Coords found for mesh");
			ret = false;
		}

		if (ret == false) //invalild mesh, discart 
		{
			if(vert!=nullptr)
				RELEASE_ARRAY(vert);
			if (indices != nullptr)
				RELEASE_ARRAY(indices);
			if (uv != nullptr)
				RELEASE_ARRAY(uv);
	

			LOG("Error loading mesh");
		}
		else 
		{
			ComponentMeshRenderer* new_mesh = (ComponentMeshRenderer*)owner->AddEmptyComponent(COMPONENT_MESH_RENDERER);
			meshes.push_back(new_mesh);

			ComponentMeshRenderer* aux = new ComponentMeshRenderer(num_vertices, vert, num_indices, indices, num_uv, uv);
			new_mesh->SetNewMesh(aux);
			//Set corresponding material
			ComponentMaterial* material = (ComponentMaterial*)owner->AddEmptyComponent(component_type::COMPONENT_MATERIAL);
			material->Set(mats[mesh->mMaterialIndex]);

			SaveToLibrary(new_mesh);
		}
	}

	return ret;
}

void MeshRendererImporter::SaveToLibrary(ComponentMeshRenderer * mesh)
{
	// Save Mesh to file
	// file structure has: 
	// amount of indices / vertices / texture_coords
	// indices / vertices / texture_coords data

	uint elements_num[3] = { mesh->GetNumIndices(), mesh->GetNumVertices() * 3, mesh->GetNumUVS() };
	uint total_size = sizeof(elements_num) + sizeof(uint)*mesh->GetNumIndices() + sizeof(float)*(mesh->GetNumVertices() * 3 + mesh->GetNumUVS() * 3);

	char* data = new char[total_size];
	char* cursor = data;

	//store the number of elements of each type
	uint size = sizeof(elements_num);
	memcpy(cursor, elements_num, size);
	cursor += size;

	//store indices
	size = sizeof(uint)*elements_num[0];
	memcpy(cursor, mesh->GetIndices(), size);
	cursor += size;

	//store vertices
	size = sizeof(float)*elements_num[1];
	memcpy(cursor, mesh->GetVertices(), size);
	cursor += size;

	//store texture_coords
	size = sizeof(float)*elements_num[2];
	memcpy(cursor, mesh->GetUVS(), size);
	cursor += size;

	//Serialize data to file
	char file[69];
	sprintf_s(file, "Library\\Meshes\\mesh_%d.ownmesh", save_id++);

	FILE* f = fopen(file, "wb");
	fwrite(data, sizeof(char), total_size, f);
	fclose(f);

	RELEASE_ARRAY(data);

	LOG("Created %s.", file);
}

void MeshRendererImporter::LoadFile(const char * path)
{
	//Open file path and get size
	FILE* file = fopen(path, "rb");
	fseek(file, 0L, SEEK_END);
	uint total_size = ftell(file);
	rewind(file); //go back to file begining

				  // Copy file to a data buffer
	char* data = new char[total_size];
	char* cursor = data;
	fread(data, sizeof(char), total_size, file);
	fclose(file);

	//Start reading num_elements for indices/vertices/texture_coords
	uint elements_num[3];
	uint size = sizeof(elements_num);
	memcpy(elements_num, cursor, size);
	cursor += size;

	//Read indices
	uint* ind = new uint[elements_num[0]];
	size = sizeof(uint)*elements_num[0];
	memcpy(ind, cursor, size);
	cursor += size;

	//Read vertices
	float* vert = new float[elements_num[1]];
	size = sizeof(float)*elements_num[1];
	memcpy(vert, cursor, size);
	cursor += size;

	//Read texture coords
	float* tex_coords = new float[elements_num[2]];
	size = sizeof(float)*elements_num[2];
	memcpy(tex_coords, cursor, size);
	cursor += size;

	//Create a mesh with this info
	ComponentMeshRenderer* geo = new ComponentMeshRenderer(elements_num[1] / 3, vert, elements_num[0], ind, elements_num[2] / 3, tex_coords);
	meshes.push_back(geo); 

	

	RELEASE_ARRAY(data);
}

void MeshRendererImporter::RemoveMesh(ComponentMeshRenderer * m)
{
	for (std::list<ComponentMeshRenderer*>::iterator mesh = meshes.begin(); mesh != meshes.end(); ++m)
	{
		if (*mesh == m)
		{
			(*mesh)->CleanUp();
			RELEASE(*mesh);

			meshes.erase(mesh);
			break;
		}
	}
}

