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
		scene_file = App->json->CreateNewJSON("Library//Scenes//std_scene.json");
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

bool SceneImporter::SavePrefab(const char* name, const char* extension, const char* path, GameObject* go)
{
	bool ret = false;

	LOG("Saving prefab with name: %s", name);


	string filepath = path;
	filepath += name;
	filepath += ".";
	filepath += extension;

	json_file* prefab = App->json->LoadJSONFile(filepath.c_str());

	if (prefab == nullptr)
		prefab = App->json->CreateNewJSON(filepath.c_str());

	if (prefab != nullptr)
	{
		prefab->Delete();

		vector<GameObject*> game_objects;
		App->scene_intro->RecursiveGetGameObjectTree(go, game_objects);

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			App->scene_intro->AddRelationGo((*it));
		}

		// Store GameObjects
		prefab->SetArray("GameObjects");

		for (vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); it++)
		{
			prefab->MoveToRoot();

			if ((*it) != App->scene_intro->GetRoot())
			{
				json_file go_node = prefab->GetNode();

				// Add and move to a new secion on the gameobjects array
				go_node.AddSectionToArray("GameObjects");
				go_node.MoveToSectionFromArray("GameObjects", go_node.GetArraySize("GameObjects") - 1);

				// Set relation id
				go_node.SetInt("id", App->scene_intro->GetRelationGo((*it)));

				// Set parent relation id
				if ((*it)->GetParent() != nullptr && (*it)->GetParent() != App->scene_intro->GetRoot())
					go_node.SetInt("parent_id", App->scene_intro->GetRelationGo((*it)->GetParent()));
				else
					go_node.SetInt("parent_id", -1);

				(*it)->Serialize(go_node);
			}
		}
		prefab->Save();

		ret = true;
	}

	App->scene_intro->ClearRelations();

	return ret;
}
void SceneImporter::SaveNewScene(const char * scene_name)
{
	const char* a = App->file_system->GetScenePath().c_str();
	SavePrefab(scene_name, "json", App->file_system->GetScenePath().c_str(), App->scene_intro->GetRoot());
}

void SceneImporter::LoadScene(const char * scene_name, bool set_scene_title)
{
	DestroyScene();
	string filepath = App->file_system->GetScenePath();
	filepath += scene_name;
	filepath += ".scene";

	GameObject* go = nullptr;
	LoadPrefab(filepath.c_str(), go);
}
void SceneImporter::DestroyScene()
{
	App->scene_intro->DestroyAllGameObjectsNow();
}
bool SceneImporter::LoadPrefab(const char * file_path, GameObject *& loaded_go)
{
	bool ret = false;

	string file_name = App->file_system->GetFileName(file_path);

	LOG("Loading prefab with name: %s", file_name.c_str());

	App->scene_intro->ClearRelations();

	json_file* prefab = App->json->LoadJSONFile(file_path);

	if (prefab != nullptr)
	{
		ret = true;

		// Load GameObjects
		int game_objects_count = prefab->GetArraySize("GameObjects");
		for (int i = 0; i < game_objects_count; i++)
		{
			json_file go_node = prefab->GetNode();
			go_node.MoveToSectionFromArray("GameObjects", i);

			int id = go_node.GetInt("id", -1);
			int parent_id = go_node.GetInt("parent_id", -1);
			GameObject* go = App->scene_intro->CreateNewGO();

			App->scene_intro->AddRleationIdGo(id, go, parent_id);

			if (go != nullptr)
				go->Serialize(go_node);

			if (i == 0)
				loaded_go = go;
		}

		for (vector<Relation>::iterator it = App->scene_intro->relations.begin(); it != App->scene_intro->relations.end(); ++it)
		{
			GameObject* parent = App->scene_intro->GetRelationId((*it).id_parent);

			if (parent != nullptr)
				parent->PushChild((*it).go);
		}

		prefab->MoveToRoot();
	}

	App->scene_intro->ClearRelations();

	return ret;
}