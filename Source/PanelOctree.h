#pragma once

#include "imgui.h"
#include "MathGeoLib\MathGeoLib.h"

class PanelOctree
{
public:
	void Start(); 
	void DrawPanel(); 

private:
	bool oct_active = false; 

	float min_point[3];
	float max_point[3];
}; 
