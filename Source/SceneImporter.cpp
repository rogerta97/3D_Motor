#include "Module.h"
#include "SceneImporter.h"
#include "JSON.h"
#include "ModuleSceneIntro.h"
#include "Application.h"

SceneImporter::SceneImporter(bool start_enabled) : Module(start_enabled)
{
}

SceneImporter::~SceneImporter()
{
}

void SceneImporter::SaveScene(json_file* file)
{
	file->SetArray("GameObjects");
	file->SetArray("Components");

	App->scene_intro->Serialize(file);
}
void SceneImporter::LoadSceneFromBuffer()
{
	if (scene_file != nullptr)
	{
		//reset scene
			//scene->CleanUp();
			//scene->Start();

		for (int i = 0; i < scene_file->GetArraySize("GameObjects"); ++i)
		{
			scene_file->MoveToSectionFromArray("GameObjects", i);
			bool active = scene_file->GetBool("active");
			bool is_static = scene_file->GetBool("is_static");
			uint new_child_id = scene_file->GetInt("new_child_id",0);
			uint parent = scene_file->GetInt("parent",0);
			std::string name = scene_file->GetString("name");
			uint uid = scene_file->GetInt("UID",0);
		
			if (parent == 0)
			{
				GameObject* go = new GameObject();
				App->scene_intro->AddGameObject(go);
				go->SetStatic(is_static);
				go->SetID(uid);
				go->SetName(name.c_str());
				go->SetActive(active);

			}
			else
			{
				GameObject* p = App->scene_intro->GetGameObjectWithUID(parent);
				GameObject* go = new GameObject();
				p->PushChild(go);
				go->SetID(uid);
				go->SetParent(p);
				go->SetName(name.c_str());
				go->SetActive(active);
				go->SetStatic(is_static);
			}
		}

		//LOAD COMPONENTS TODO: needs resource manager
	}
}

