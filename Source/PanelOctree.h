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

	float3 min_point;
	float3 max_point;

	int max_objects = 2; 
	int obj_amount = 0; 
}; 
