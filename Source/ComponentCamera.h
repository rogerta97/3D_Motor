#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"
#include "glmath.h"
#include "DebugDraw.h"

class Color;
class json_file;

class ComponentCamera : public Component
{
public: 

	ComponentCamera(GameObject* parent, float far_plane = 0.1f, float near_plane = 300.0f, float fov = 45.0f, float aspect_ratio = 1.3f);

	~ComponentCamera(); 

	//Parent
	bool Update(); 
	void OnLoad(json_file* config);
	void OnSave(json_file& config)const;
	void DeleteComponent();

	void DrawFrustum(Frustum & frustum, Color color);

	//--frustrum get&set functions
	float GetNearPlaneDist() const;
	float GetFarPlaneDist() const;
	float GetFOV() const;
	float GetAspectRatio() const;

	void SetNearPlaneDist(float dist);
	void SetFarPlaneDist(float dist);
	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);

	// Move 
	void MoveForward(const float& speed);
	void MoveBackwards(const float& speed);
	void MoveRight(const float& speed);
	void MoveLeft(const float& speed);
	void MoveUp(const float& speed);
	void MoveDown(const float& speed);

	// transform rotation and scale
	void SetPosition(const float3& pos);
	void Orbit(const float3& point, const float& motion_x, const float& motion_y);
	void Rotate(const float& motion_x, const float& motion_y);
	void Focus(const float3& point, const float& distance);

	void Look(const float3& pos);
	bool IsInside(AABB& bbox);	

	void GetObjectsOnFrustum(vector<GameObject*>& objects);

	float* GetOpenGLViewMatrix()const;
	float* GetOpenGLProjectionMatrix()const;

public:

	Frustum frustum;

	bool frustum_culling = false; 
	bool screen_resized = false;
};

#endif
