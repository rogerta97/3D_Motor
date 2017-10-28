#ifndef _PANELIHIERARCHY_H_
#define _PANELIHIERARCHY_H_

#include "imgui.h"

class GameObject; 

class PanelHierarchy
{
public:

	bool Draw();

	void DrawNode(GameObject* go); 


private:

	bool show_hierarchy = true;

};

#endif
