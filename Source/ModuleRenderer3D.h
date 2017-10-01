#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void PrintConfigData();

	char* GetGraphicsModel(const char* _module); 
	const char* GetDriver();
	void SetObjectsWireframe(bool state); 

private: 
	
	bool depth_test_ch_b = true; 
	bool cull_face_ch_b = true;
	bool lighting_ch_b = true;
	bool color_ch_b = true;
	bool texture_ch_b = true;
	bool wireframe_ch_b = false; 
	bool fog_ch_b = false;

public:

	Light lights[MAX_LIGHTS]; 

	float fog_density; 

	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

};