#include "Module.h"
#include "SceneImporter.h"
#include "JSON.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "GameObject.h"

SceneImporter::SceneImporter(bool start_enabled) : Module(start_enabled)
{
}

SceneImporter::~SceneImporter()
{
}

bool SceneImporter::Start()
{
	App->CreateFolder("Assets\\Prefabs");
	App->CreateFolder("Library\\Prefabs");

	App->CreateFolder("Assets\\Scenes");
	App->CreateFolder("Library\\Scenes");
	return true;
}

void SceneImporter::SaveScene(json_file* file)
{
	file->SetArray("GameObjects");
	file->SetArray("Components");

	App->scene_intro->Serialize(file);
	file->Save();

}
void SceneImporter::SaveScene(const char * path)
{
	json_file* doc = App->json->LoadJSONFile(path);

	if (doc == nullptr)
		doc = App->json->CreateNewJSON(path);
	else
		doc->Delete();

	doc->SetArray("GameObjects");
	doc->SetArray("Components");

	App->scene_intro->Serialize(doc);
	doc->Save();
}
void SceneImporter::SaveSceneToBuffer()
{
	if (scene_file == nullptr)
		scene_file = App->json->CreateNewJSON("scene.json");
	else
		scene_file->Delete();

	SaveScene(scene_file);
}

void SceneImporter::LoadSceneFromBuffer()
{
	scene_file = App->json->LoadJSONFile("scene.json");
	GameObject* ret;
	if (scene_file != nullptr)
	{
		for (int i = 0; i < scene_file->GetArraySize("GameObjects"); ++i)
		{
			scene_file->MoveToSectionFromArray("GameObjects", i);
			bool active = scene_file->GetBool("active");
			bool is_static = scene_file->GetBool("is_static");
			uint parent = scene_file->GetInt("parent",0);
			std::string name = scene_file->GetString("name");
			uint uid = scene_file->GetInt("UID",0);
			uint new_child_id = scene_file->GetInt("new_child_id", 0);

			if (parent == 0)
			{
				ret = new GameObject();
				App->scene_intro->AddGameObject(ret);
				ret->SetStatic(is_static);
				ret->SetID(uid);
				ret->SetName(name.c_str());
				ret->SetActive(active);
				ret->SetNewChildID(new_child_id);
			}
			else
			{
				GameObject* p = (parent == ret->GetID()) ? ret : nullptr;
				if (p == nullptr) ret->GetChildByUID(parent, p);
				GameObject* go = new GameObject();
				p->PushChild(go);
				go->SetID(uid);
				go->SetParent(p);
				go->SetName(name.c_str());
				go->SetActive(active);
				go->SetStatic(is_static);
				go->SetNewChildID(new_child_id);
			}
			scene_file->MoveToRoot();
		}
		//LOAD COMPONENTS
		for (int i = 0; i < scene_file->GetArraySize("Components"); ++i)
		{
			scene_file->MoveToSectionFromArray("Components", i);

			component_type t = static_cast<component_type>((int)scene_file->GetInt("type",0));
			uint owner = scene_file->GetUInt("owner",0);
			GameObject* go = (owner == ret->GetID()) ? ret : nullptr;
			if (go == nullptr) ret->GetChildByUID(owner, go);

			switch (t)
			{
			case COMPONENT_TRANSFORM:
			{
				const float3 pos = scene_file->GetFloat3("position");
				float3 scale = scene_file->GetFloat3("scale");
				Quat rotation = scene_file->GetQuat("rotation");
				

				ComponentTransform* t = (ComponentTransform*)go->AddEmptyComponent(COMPONENT_TRANSFORM);
				//Set pos/rot/scale
				break;
			}
			case COMPONENT_MESH_RENDERER:
			{
				uint mesh_uid = scene_file->GetUInt("mesh",0);

				ComponentMeshRenderer* mr = (ComponentMeshRenderer*)go->AddEmptyComponent(COMPONENT_MESH_RENDERER);
				
				//mr->
				//mr->SetMesh(mesh_uid);
				break;
			}
			case COMPONENT_MATERIAL:
			{
				uint mat_uid = scene_file->GetUInt("material",0);

				ComponentMaterial* m = (ComponentMaterial*)go->AddEmptyComponent(COMPONENT_MATERIAL);
				//m->SetMaterial(mat_uid);
	

				break;
			}
			case COMPONENT_CAMERA:
			{
				float aspect = scene_file->GetFloat("aspect_ratio",0);
				float fov = scene_file->GetFloat("fov",0);
				float near_plane = scene_file->GetFloat("near",0);
				float far_plane = scene_file->GetFloat("far",0);

				ComponentCamera* c = (ComponentCamera*)go->AddEmptyComponent(COMPONENT_CAMERA);
				c->SetFOV(fov);
				c->SetNearPlaneDist(near_plane);
				c->SetAspectRatio(aspect);
				c->SetFarPlaneDist(far_plane);
				break;
			}
			}

			scene_file->MoveToRoot();
		}
	}
}


