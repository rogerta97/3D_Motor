#ifndef _PANELINSPECTOR_H_
#define _PANELINSPECTOR_H_

#include "imgui.h"

class PanelInspector
{
public: 

	bool Start(); 

	bool Update(); 

	bool Draw(); 


private:

	bool show_inspector = true; 
	bool show_aabb = false; 

};

#endif
