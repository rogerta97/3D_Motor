#include "ComponentCamera.h"
#include "Color.h"
#include "DebugDraw.h"

ComponentCamera::ComponentCamera()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = DEGTORAD * 45.0f;
	SetAspectRatio(1.3f);
	type = COMPONENT_CAMERA; 
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update()
{
	DrawFrustum(frustum, Color(1.0f, 1.0f,0.0f));
	return true;
}

void ComponentCamera::DrawFrustum(Frustum & frustum, Color color)
{
	float3 frustum_vertices[8]; 
	frustum.GetCornerPoints(frustum_vertices);
	
	DebugDrawBox(frustum_vertices, color);
}

float ComponentCamera::GetNearPlaneDist() const
{
	return frustum.farPlaneDistance;
}

float ComponentCamera::GetFarPlaneDist() const
{
	return frustum.farPlaneDistance;
}

float ComponentCamera::GetFOV() const
{
	return frustum.verticalFov * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return frustum.AspectRatio();
}

void ComponentCamera::Look(const float3& position)
{
	float3 dir = position - frustum.pos;

	float3x3 m = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = m.MulDir(frustum.front).Normalized();
	frustum.up = m.MulDir(frustum.up).Normalized();
}

void ComponentCamera::SetNearPlaneDist(float dist)
{
	if (dist > 0)
	{
		frustum.nearPlaneDistance = dist;
		screen_resized = true;
	}
}

void ComponentCamera::SetFarPlaneDist(float dist)
{
	if (dist > frustum.nearPlaneDistance  && dist>0.0f)
	{
		frustum.farPlaneDistance = dist;
		screen_resized = true;
	}
}

void ComponentCamera::SetFOV(float fov)
{
	float aspect_ratio = frustum.AspectRatio();
	frustum.verticalFov = DEGTORAD * fov;
	SetAspectRatio(aspect_ratio);
}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect_ratio);
	screen_resized = true;
}

float * ComponentCamera::GetOpenGLViewMatrix()
{
	float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();
	return (float*)matrix.v;
}

float * ComponentCamera::GetOpenGLProjectionMatrix()
{
	float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();
	return (float*)matrix.v;
}
