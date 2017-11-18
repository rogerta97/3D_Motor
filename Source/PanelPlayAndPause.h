#pragma once

#include "imgui.h"

class PanelPlayAndPause
{
public: 
	PanelPlayAndPause(); 

	void Draw(); 

	~PanelPlayAndPause(); 

private: 
	bool show_playpause = true; 

	bool play_presed = false; 
	bool pause_presed = false; 
	bool stop_presed = false; 
};
