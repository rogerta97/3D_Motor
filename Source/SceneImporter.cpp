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

	if (scene_file != nullptr)
	{
		//App->scene_intro->CleanUp();
		//App->scene_intro->Start();

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
				GameObject* go = new GameObject();
				App->scene_intro->AddGameObject(go);
				go->SetStatic(is_static);
				go->SetID(uid);
				go->SetName(name.c_str());
				go->SetActive(active);
				go->SetNewChildID(new_child_id);
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
				go->SetNewChildID(new_child_id);
			}
			scene_file->MoveToRoot();
		}
	}
}

