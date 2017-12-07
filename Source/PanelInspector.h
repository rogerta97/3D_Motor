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

private:

	void PrintMeshComponent(GameObject* GO_to_draw); 
	void PrintMaterialComponent(GameObject* GO_to_draw);
	void PrintTransformComponent(GameObject* GO_to_draw);
	void PrintCameraComponent(GameObject* GO_to_draw);
	void PrintBillBoardingComponent(GameObject* Go_to_draw); 
	void PrintComponentParticleEmmiter(GameObject* Go_to_draw);

	void ShowMaterialResources(); 

private:

	bool show_inspector = true; 
	bool show_component_popup = false; 
	bool show_resources_popup = false; 

};

#endif
