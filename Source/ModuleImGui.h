#pragma once

#include "Module.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelPlayAndPause.h"
#include "PanelModalWindow.h"
#include "PanelOctree.h"
#include "PanelExplorer.h"
#include "ImGuizmo.h"
#include "RayCast.h"

#include<vector>
class ModuleImGui : public Module
{
public: 

	ModuleImGui(bool start_enabled = true); 
	~ModuleImGui();

	bool Init(json_file* config);
	update_status PreUpdate(float dt); 
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp(); 

	void AddInConsole(const std::string);
	void ImGuiInput(SDL_Event* e)const; 
	void SetInitColors();
	void StartModalWindow(const char * text, const char* OK_button, const char* NO_button, const char* window_title);
	
	modal_state GetModalState(); 
	void SetModalState(modal_state new_st); 

private:

	void PrintProperties();
	void PrintRandomNumber();
	void ShowAbout();
	void UpdateConfigPanel(); 
	void PrintStyleEditor(); 

	update_status PrintTopBar(); 

private: 

	PanelInspector inspector;
	PanelConsole console;
	PanelHierarchy hierarchy; 
	PanelOctree panel_octree; 
	PanelPlayAndPause panel_play; 
	PanelExplorer panel_explorer; 
	PanelModalWindow panel_modal_window; 

	RayCast raycast; 

	bool show_test_window = true;
	bool show_another_window = false;
	bool show_console = true; 
	bool show_random_number = false;
	bool show_properties = true;
	bool show_about = false; 
	bool show_gui_demo = false; 
	bool show_performance = false; 
	bool show_configuration = true; 
	bool show_inspector = false;
	bool show_hierarchy = true; 
	bool show_octree_window = false;
	bool show_style_editor = false; 
	bool show_modal_window = false; 

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
	//ImGuizmo operation and mode
	ImGuizmo::OPERATION curr_operation;
	ImGuizmo::MODE curr_mode;

};
