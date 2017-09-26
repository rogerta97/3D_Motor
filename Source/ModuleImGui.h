#pragma once

#include "Module.h"
#include<vector>
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

	void AddToConsole(const char* new_line);
	void ClearConsole(); 

private:

	void PrintConsole(); 
	void ShowAbout(); 
	void UpdateConfigPanel(); 

private: 

	bool show_test_window = true;
	bool show_another_window = false;
	bool show_console = false; 
	bool show_about = false; 
	bool show_gui_demo = false; 

	// Console management -----------

	std::vector<const char*> console_buffer; 

	const char* new_input = ""; 

	// ------

	ImVec4 clear_color = ImColor(114, 144, 154);

};
