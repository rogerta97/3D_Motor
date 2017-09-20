#pragma once

#include "Module.h"
#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

class ModuleImGui : public Module
{
public: 

	ModuleImGui(Application* parent, bool start_enabled = true); 
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt); 
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp(); 

private: 

	bool show_test_window = true;
	bool show_another_window = false;
	bool show_console = false; 

	

	ImVec4 clear_color = ImColor(114, 144, 154);

};
