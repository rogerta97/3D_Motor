#include "Globals.h"
#include "Application.h"
#include "ModuleFBXLoader.h"
#include "ModuleSceneIntro.h"
#include "ComponentDefs.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include <vector>

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

	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
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

void ModuleFBXLoader::LoadFileScene(const char * path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	aiNode* root_node = scene->mRootNode; 

	LoadFBX(path, root_node, scene, nullptr);

	aiReleaseImport(scene);

}

void ModuleFBXLoader::GetInnerTransform(aiNode & node, aiVector3D & pos, aiQuaternion & quat, aiVector3D & scale)
{
	std::string node_name(node.mName.C_Str());

	if (node.mNumChildren > 0)
	{
		if (node_name.find("$AssimpFbx$_PreRotation") != std::string::npos || node_name.find("$AssimpFbx$_Rotation") != std::string::npos ||
			node_name.find("$AssimpFbx$_PostRotation") != std::string::npos || node_name.find("$AssimpFbx$_Scaling") != std::string::npos ||
			node_name.find("$AssimpFbx$_Translation") != std::string::npos)
		{

			aiVector3D new_pos;
			aiQuaternion new_quat;
			aiVector3D new_scale;

			node.mTransformation.Decompose(new_scale, new_quat, new_pos);

			node = *node.mChildren[0]; 

			pos += new_pos; 
			quat = quat * new_quat; 

			aiVector3D n_scale(scale*new_scale); 
			scale = n_scale;
			GetInnerTransform(node, pos, quat, scale);
		}
	}


}

void ModuleFBXLoader::LoadFBX(const char* full_path, aiNode* node, const aiScene* scene, GameObject* parent)
{
	bool ret = true; 

	GameObject* new_go = nullptr; 

	if (node->mNumMeshes < 1)
	{		
		std::string node_name(node->mName.C_Str());

		aiVector3D pos;
		aiQuaternion quat;
		aiVector3D scale;

		if (node_name.find("$AssimpFbx$_PreRotation") != std::string::npos || node_name.find("$AssimpFbx$_Rotation") != std::string::npos ||
			node_name.find("$AssimpFbx$_PostRotation") != std::string::npos || node_name.find("$AssimpFbx$_Scaling") != std::string::npos ||
			node_name.find("$AssimpFbx$_Translation") != std::string::npos)
		{
			GetInnerTransform(*node, pos, quat, scale);
		}
		else
		{
			node->mTransformation.Decompose(scale, quat, pos); 
		}

		new_go = new GameObject();

		node_name = node->mName.C_Str();

		if (node_name == "RootNode")
		{
			// A function needs to be created for this

			std::string full_path_str(full_path);
			uint cut1 = full_path_str.find_last_of('\\');
			uint cut2 = full_path_str.find_last_of('.');

			uint lenght = cut2 - cut1; 

			std::string final_str = full_path_str.substr(cut1 + 1, lenght - 1);
			node_name = final_str; 

			 // -----------------

		}
			new_go->SetName(node_name.c_str());

		if (parent != nullptr) parent->PushChild(new_go);

		ComponentTransform* trans = (ComponentTransform*)new_go->GetComponent(COMPONENT_TRANSFORM); 

		float3 n_pos(pos.x, pos.y, pos.z);
		float3 n_scale(scale.x, scale.y, scale.z);
		Quat n_rot(quat.x, quat.y, quat.z, quat.w);

		trans->SetPosition(n_pos);
		trans->SetRotation(n_rot);
		trans->SetScale(n_scale);
		
		App->scene_intro->AddGameObject(new_go);
	}
	else
	{
		if (scene != nullptr && scene->HasMeshes())
		{
			// Use scene->mNumMeshes to iterate on scene->mMeshes array
			
			for (int i = 0; i < node->mNumMeshes; i++)
			{
				//Vertices
				std::string node_name(node->mName.C_Str());

				GameObject* child_go = new GameObject();
				child_go->SetName(node_name.c_str()); 

				LOG("Loading new mesh...")

				aiMesh* m = scene->mMeshes[node->mMeshes[i]];

				ComponentMeshRenderer* tmp_mr = new ComponentMeshRenderer(child_go);

				tmp_mr->num_vertices = m->mNumVertices;
				tmp_mr->vertices = new float[tmp_mr->num_vertices * 3];
				memcpy(tmp_mr->vertices, m->mVertices, sizeof(float) * tmp_mr->num_vertices * 3);

				glGenBuffers(1, (GLuint*)&tmp_mr->vertices_id);
				glBindBuffer(GL_ARRAY_BUFFER, tmp_mr->vertices_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_mr->num_vertices * 3, tmp_mr->vertices, GL_STATIC_DRAW);

				LOG("%d vertices", tmp_mr->num_vertices);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				//Indices

				if (m->HasFaces()) {
					tmp_mr->num_indices = m->mNumFaces * 3;
					tmp_mr->indices = new uint[tmp_mr->num_indices];

					for (uint i = 0; i < m->mNumFaces; ++i)
					{
						if (m->mFaces[i].mNumIndices != 3) {
							LOG("WARNING, geometry face with != 3 indices!");
						}
						else
							memcpy(&tmp_mr->indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
					}

				}

				glGenBuffers(1, (GLuint*)&tmp_mr->indices_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmp_mr->indices_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * tmp_mr->num_indices, tmp_mr->indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				LOG("%d indices", tmp_mr->num_indices);

				if (m->HasTextureCoords(0)) // assume mesh has one texture coords
				{
					tmp_mr->num_uvs = m->mNumVertices;
					tmp_mr->uvs = new float[tmp_mr->num_uvs * 3];
					memcpy(tmp_mr->uvs, m->mTextureCoords[0], sizeof(float)*tmp_mr->num_uvs * 3);

					glGenBuffers(1, (GLuint*)&tmp_mr->uvs_id);
					glBindBuffer(GL_ARRAY_BUFFER, (GLuint)tmp_mr->uvs_id);
					glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * tmp_mr->num_uvs * 3, tmp_mr->uvs, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					LOG("%d texture cordinates", tmp_mr->num_uvs);
				}
				else
				{
					LOG("No Texture Coords found");
				}
				AABB bbox;
				bbox.SetNegativeInfinity();
				bbox.Enclose((float3*)m->mVertices, m->mNumVertices);

				tmp_mr->SetGizmoBox(bbox);
				//FIX ME
				//App->camera->Focus(vec3(new_object->GetPosition().x, new_object->GetPosition().y, new_object->GetPosition().z), bbox.Size().Length() *1.2f);

				tmp_mr->type = COMPONENT_MESH_RENDERER;
				tmp_mr->Enable();

				tmp_mr->SetComponentParent(child_go);
				tmp_mr->tranform_id = i;

				child_go->PushComponent((Component*)tmp_mr);
				//	glBindBuffer(GL_ARRAY_BUFFER, 0);

				LOG("FBX imported with %d meshes", i);

				if (scene != nullptr && scene->HasMaterials())
				{
					aiMaterial* mat = scene->mMaterials[m->mMaterialIndex];
					aiString path;

					ComponentMaterial* MA_tmp;
					mat->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					std::string full_path_str(full_path);
					uint cut = full_path_str.find_last_of('\\');

					std::string final_str = full_path_str.substr(0, cut + 1);
					final_str += path.C_Str();
					MA_tmp = ImportImage(final_str.c_str());

					MA_tmp->type = COMPONENT_MATERIAL;
					MA_tmp->Enable();

					MA_tmp->SetComponentParent(child_go);

					child_go->PushComponent(MA_tmp);
				}

				if (node != nullptr)
				{
					ComponentTransform* TR_cmp = (ComponentTransform*)child_go->GetComponent(COMPONENT_TRANSFORM);

					aiVector3D translation;
					aiVector3D scaling;
					aiQuaternion rotation;

					node->mTransformation.Decompose(scaling, rotation, translation);

					float3 pos(translation.x, translation.y, translation.z);
					float3 scale(scaling.x, scaling.y, scaling.z);
					Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);


					TR_cmp->SetPosition(pos);
					TR_cmp->SetRotation(rot);
					TR_cmp->SetScale(scale);

					TR_cmp->type = COMPONENT_TRANSFORM;
					TR_cmp->Enable();

					LOG("FBX imported with %d transform", i);
				}

				parent->PushChild(child_go);
				App->scene_intro->AddGameObject(child_go);
			}

			new_go = parent;
		}
		else
		{
			LOG("Error loading scene %s", full_path);
		}
	}
		

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadFBX(full_path, node->mChildren[i], scene, new_go);
	}

	App->scene_intro->SetCurrentGO(new_go->GetID()); 
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
//void ModuleFBXLoader::GetMaterialInfo(int w,int h)
//{
//
//}
ComponentMaterial* ModuleFBXLoader::ImportImage(const char * path)
{
	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ComponentMaterial* new_component = new ComponentMaterial(nullptr);

	success = ilLoadImage(path);

	if (success)
	{
		LOG("Loading new texture with path %s", path); 

		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
	
		new_component->width = ilGetInteger(IL_IMAGE_WIDTH);
		new_component->height = ilGetInteger(IL_IMAGE_HEIGHT);
		new_component->type = COMPONENT_MATERIAL;
			
		if (!success)
		{
			LOG("Image conversion failed: %s\n", ilGetError());
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		new_component->textures_id = textureID; 

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());


	}
	else
	{
		textureID = 0;
		LOG("Not image found in %s", path); 
	}

	return new_component;
}

//void Mesh::Clean()
//{
//	//delete(vertices);
//	//vertices = nullptr; 
//
//	//delete(indices); 
////	indices = nullptr;
//}
