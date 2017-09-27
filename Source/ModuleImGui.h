#pragma once

#include "Module.h"
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

	void AddToConsole(std::string new_line);
	void ClearConsole(); 
	void ImGuiInput(SDL_Event* e); 

private:

	void PrintConsole(); 
	void PrintRandomNumber();
	void ShowAbout(); 
	void UpdateConfigPanel(); 

private: 

	bool show_test_window = true;
	bool show_another_window = false;
	bool show_console = false; 
	bool show_random_number = false;
	bool show_about = false; 
	bool show_gui_demo = false; 

	// Console management -----------

	std::vector<std::string> console_buffer; 
	int i_rand = 60;
	int i_max = 100;
	int i_min = 0;
	float f_rand = 0.0f;
	const char* new_input = ""; 

	// ------

	ImVec4 clear_color = ImColor(114, 144, 154);

};
