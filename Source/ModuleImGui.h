#pragma once

#include "Module.h"
#include "PanelConsole.h"
#include "PanelInspector.h"

#include<vector>



class ModuleImGui : public Module
{
public: 

	ModuleImGui(bool start_enabled = true); 
	~ModuleImGui();

	bool Init();
	update_status PreUpdate(float dt); 
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp(); 

	void AddInConsole(const std::string);
	void ImGuiInput(SDL_Event* e)const; 

private:

	void PrintProperties();
	void PrintRandomNumber();
	void PrintHierarchy();
	void ShowAbout();
	void UpdateConfigPanel(); 
	update_status PrintTopBar(); 

private: 

	PanelInspector inspector;
	PanelConsole console;

	bool show_test_window = true;
	bool show_another_window = false;
	bool show_console = false; 
	bool show_random_number = false;
	bool show_properties = true;
	bool show_about = false; 
	bool show_gui_demo = false; 
	bool show_performance = false; 
	bool show_configuration = true; 
	bool show_inspector = false;
	bool show_hierarchy = true; 

	// Console management -----------

	int i_rand = 60;
	int i_max = 100;
	int i_min = 0;
	float f_rand = 0.0f;
	const char* new_input = "";

	// ------

	//drawing positions //SDL_Rect doesn't work at allneed to change it 
	//panel
	uint properties_panel_x = 10;
	uint properties_panel_y = 10;
	uint properties_panel_w = 100;
	uint properties_panel_h = 200;

	ImVec4 clear_color = ImColor(114, 144, 154);



};
