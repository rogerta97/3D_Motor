#pragma once

#include "imgui.h"
#include "Globals.h"

class PanelModalWindow
{
public:
	PanelModalWindow(); 
	~PanelModalWindow(); 

	void SetData(const char* text, const char* yes_text, const char* no_text, const char* window_title);
	
	modal_state GetState(); 
	void SetState(modal_state new_st);

	void Update(); 

private: 
	const char* text; 
	const char* yes_button_text; 
	const char* no_button_text; 

	const char* window_title; 

	modal_state curr_state; 
};
