#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModuleImGui.h"
#include "MathGeoLib\MathGeoLib.h"
#include "DeviceId.h"
#include <queue>
#include <string>
#include <list>


class Application
{
public:
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
	Timer			global_timer; 
	Timer			frame_ms_timer; 

	float			dt = 0.0f;
	std::list<Module*> list_modules;

	// Framerate management --------------

	int	last_sec_frame_counter = 0;
	int global_frames = 0; 
	
	std::vector<float>	framerate_buffer; 
	std::vector<float>	miliseconds_buffer;

	int framerate_placer = 0; 
	int framerate_avg = 0; 

	// -----------------------------------

	// Hardware ----------------------

	//int vendor;
	//int devide; 
	//int brand; 
	//int total_vram;
	//int used_vram; 
	//int avaliable_vram;
	//int reserved_vram; 

	uint vendorid, deviceid;
	Uint64 vm, vm_curr, vm_a, vm_r;
	std::wstring brand;

	// ------------------------------

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	void PrintConfigData();
	bool CleanUp();

	Module* GetModule(int index);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};