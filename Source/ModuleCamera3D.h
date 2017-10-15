#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(); 
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	void Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y);
	void Focus(const vec3& focus, const float& distance);
	void PrintConfigData();

private:
	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	float mov_speed = 0.1f;
	float rot_speed = 0.25f;
	float zm_speed = 0.1f;
	float mouse_wheel_speed = 2.0f;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};