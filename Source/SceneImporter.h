#ifndef _SCENE_IMPORTER_H_
#define _SCENE_IMPORTER_H_

class Module;
class json_file;
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


	//To modify when we have more than one Scene TODO
	//call this one when clicking stop
	void LoadSceneFromBuffer();
public:
	json_file* scene_file = nullptr;


};
#endif !_SCENE_IMPORTER_H_