#pragma once

#include "imgui.h"

class PanelExplorer
{
public:

	PanelExplorer();
	~PanelExplorer(); 

	void Start(const char* root_path); 
	void Update(); 

private:
	void Draw(); 

	const char* root_path = ""; 
};
