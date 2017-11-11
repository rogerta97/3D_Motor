#include "ComponentCamera.h"
#include "Color.h"
#include "DebugDraw.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"
#include "Application.h"

ComponentCamera::ComponentCamera(GameObject* _parent, float far_plane, float near_plane, float fov, float aspect_ratio)
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = near_plane;
	frustum.farPlaneDistance = far_plane;
	frustum.verticalFov = DEGTORAD * fov;
	SetAspectRatio(aspect_ratio);
	type = COMPONENT_CAMERA; 
	
	parent = _parent; 
	active = true; 
	//App->scene_intro->cameras_list.push_back(this);

}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update()
{
	DrawFrustum(frustum, Color(1.0f, 1.0f,0.0f));

	ComponentTransform* trans = (ComponentTransform*)GetComponentParent()->GetComponent(COMPONENT_TRANSFORM);

	if (trans->IsModified())
	{
		frustum.pos = trans->GetLocalPosition(); 	
		trans->SetModified(false); 
	}

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
	//get directional vector
	float3 direction = position - frustum.pos;

	float3x3 m = float3x3::LookAt(frustum.front, direction.Normalized(), frustum.up, float3(0, 1, 0));

	frustum.front = m.MulDir(frustum.front).Normalized();
	frustum.up = m.MulDir(frustum.up).Normalized();
}

bool ComponentCamera::HasAABB(AABB & GO_bb)
{
	bool ret = false;
	int vertex_num = GO_bb.NumVertices();
	for (int i = 0; i < 6; i++) 
	{
		int outside_points = 0;
		for (int j = 0; j < vertex_num; j++)
		{
			Plane plane = frustum.GetPlane(i);
			if (plane.IsOnPositiveSide(GO_bb.CornerPoint(j)))
			{
				outside_points++;
			}
		}
		if (outside_points == 8)
			return ret;
		else
			ret = true;
	}


	return ret;
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

float * ComponentCamera::GetOpenGLViewMatrix()const
{
	float4x4 matrix;
	matrix = frustum.ViewMatrix();
	matrix.Transpose();
	return (float*)matrix.v;
}

float * ComponentCamera::GetOpenGLProjectionMatrix()const
{
	float4x4 matrix;
	matrix = frustum.ProjectionMatrix();
	matrix.Transpose();
	return (float*)matrix.v;
}
