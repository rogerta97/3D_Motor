#ifndef _PANELINSPECTOR_H_
#define _PANELINSPECTOR_H_

#include "imgui.h"

class PanelInspector
{
public: 

	bool Start(); 

	bool Update(); 

	bool Draw(); 

	bool ShowWarningModal(); 


private:

	bool show_inspector = true; 
	bool show_component_popup = false; 
	bool modal_window = false; 

};

#endif
