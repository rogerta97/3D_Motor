#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"
#include "glmath.h"
#include "DebugDraw.h"

class Color;

class ComponentCamera : public Component
{
public: 

	ComponentCamera(); 

	~ComponentCamera(); 

	bool Update(); 

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
	//---
	void Look(const float3& position);
	bool HasAABB(AABB& GO_bb);
	float* GetOpenGLViewMatrix();
	float* GetOpenGLProjectionMatrix();
public:
	Frustum frustum;
	GameObject* selected_GO = nullptr;
	bool frustum_culling = false; 
	bool screen_resized = false;



};

#endif
