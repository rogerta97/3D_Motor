#ifndef _PANELINSPECTOR_H_
#define _PANELINSPECTOR_H_

#include "imgui.h"

class GameObject; 

class PanelInspector
{
public: 

	bool Start(); 

	bool Update(); 

	bool Draw(); 

	bool ShowWarningModal(); 

private:

	void PrintMeshComponent(GameObject* GO_to_draw); 
	void PrintMaterialComponent(GameObject* GO_to_draw);
	void PrintTransformComponent(GameObject* GO_to_draw);
	void PrintCameraComponent(GameObject* GO_to_draw);

private:

	bool show_inspector = true; 
	bool show_component_popup = false; 
	bool modal_window = false; 

};

#endif
