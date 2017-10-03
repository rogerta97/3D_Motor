#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "Performance.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "MathGeoLib\MathGeoLib.h"
#include "DeviceId\DeviceId.h"
#include "JSON.h"
#include "Hardware.h"
#include <queue>
#include <string>
#include <list>

#define FPS_LOG_SIZE 100

class Application
{
public:
	JSON*		json;
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleImGui* imgui; 

private:

	Timer			ms_timer;
	Timer			fps_timer;
	

	float			dt = 0.0f;
	std::list<Module*> list_modules;

	// Framerate management --------------

	int		last_sec_frame_counter = 0;
	int		global_frames = 0; 
	int		capped_ms = 1000 / 60.0f;
	int		fps_counter = 0;
	int		last_frame_ms = 0;
	int		last_fps = 0;

	Uint64 frame_counter = 0;//we will need a large uint to store the larg value

	
	std::vector<float>	framerate_buffer; 
	std::vector<float>	miliseconds_buffer;

	int framerate_placer = 0; 
	int framerate_avg = 0; 

	// -----------------------------------

	// Hardware ----------------------

	Hardware hardware; 

	// ------------------------------


public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	void PrintConfigData();
	void LoadConfig();
	void SaveConfig(Module* module);
	bool CleanUp();
	const char* GetAppName();
	const char* GetOrgName();
	void CapFPS(float fps);
	uint GetCapFPS()const;
	void OpenWebPage(const char* url); 
	//Finish Update functions
	void DelayToCap()const;
	void UpdateBuffers(float fps_buf, float ms_buf);
	void FitHistogram();
	//add get & set config functions
	Module* GetModule(int index);

	//For performance

	PerformanceWindow performance;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	std::string		   engine_name = "-";
	std::string		   organization = "-";
	float				max_fps = 0.0f;

	json_file*			config = nullptr;
};

extern Application* App; 