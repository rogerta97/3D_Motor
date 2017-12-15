#ifndef _SCENE_IMPORTER_H_
#define _SCENE_IMPORTER_H_

class Module;
class json_file;
class GameObject;

class SceneImporter : public Module
{
public:
	SceneImporter(bool start_enabled = true);
	~SceneImporter();

	bool Start();
	//call this one when click play
	void SaveScene(json_file* file);
	void SaveScene(const char * path);
	void SaveSceneToBuffer();

	//PLAY&Pause
	void SaveNewScene(const char * scene_name);
	void LoadScene(const char * scene_name, bool set_scene_title);
	void DestroyScene();

	//To modify when we have more than one Scene TODO
	//call this one when clicking stop
	void LoadSceneFromBuffer();

	bool SavePrefab(const char* name, const char* extension, const char* path, GameObject* go);
	bool LoadPrefab(const char * file_path, GameObject *& loaded_go);

public:
	json_file* scene_file = nullptr;
	std::vector<GameObject*> resources;
};
#endif !_SCENE_IMPORTER_H_