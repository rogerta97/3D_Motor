#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	// Parent funcs

	bool Init(json_file* config);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	// Camera Control

	void Look(const float3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &objective);
	void Move();
	void Move(const vec3& pos);
	void SetCamPosition(math::float3 position);
	void FreeOrbit();
	void Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y);
	void Focus(const vec3& focus, const float& distance);

	// Matrix Management

	float* GetViewMatrix();
	float* GetFrustumViewMatrix();
	float* GetFrustumProjectionMatrix();

	// Utility

	bool IsCulling();
	void AdaptToState(app_state curr_state);

	ComponentCamera* GetEditorCam()const;
	ComponentCamera* GetMainCam()const;

	bool IsLooking4Camera(); 
	void SetLooking4Camera(bool state_Cam); 
	bool HasMainCamera(); 

	void AssignNewMainCamera(ComponentCamera* go_container); 

	// Config window

	void PrintConfigData();

private:
	void CalculateViewMatrix();
	
public:
	
	vec3 X, Y, Z, Position, Reference;

	float mov_speed = 2.5f;//we should load it from config
	float rot_speed = 0.25f;
	float zm_speed;
	float mouse_wheel_speed;

	float near_plane;
	float far_plane;
	float h_field_of_view;
	float v_field_of_view;
	float aspect_ratio;

	bool frustum_culling = false; 
	bool is_rotating = false; 
	bool looking_for_camera = false; 

private:
	mat4x4 ViewMatrix, ViewMatrixInverse;

	ComponentCamera* editor_camera = nullptr;
	ComponentCamera* main_camera = nullptr; 

};