#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(json_file* config);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &objective);
	void Move();
	void Move(const vec3& pos);
	float* GetViewMatrix();
	void FreeOrbit();
	void Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y);
	void Focus(const vec3& focus, const float& distance);
	void PrintConfigData();
	void SetCamPosition(math::float3 position);

	ComponentCamera* GetEditorCam()const;

	bool IsCulling(); 

private:
	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;

	float mov_speed;
	float rot_speed;
	float zm_speed;
	float mouse_wheel_speed;

	float near_plane;
	float far_plane;
	float field_of_view;
	float aspect_ratio;

	bool frustum_culling = false; 

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;

	ComponentCamera* editor_camera = nullptr;
	ComponentCamera* main_camera = nullptr; 

};