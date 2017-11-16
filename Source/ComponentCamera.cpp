#include "ComponentCamera.h"
#include "Color.h"
#include "DebugDraw.h"
#include "ComponentTransform.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "JSON.h"

ComponentCamera::ComponentCamera(GameObject* _parent, float far_plane, float near_plane, float fov, float aspect_ratio)
{
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL, FrustumHandedness::FrustumRightHanded);

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetFrame({0, 2, -5}, float3::unitZ, float3::unitY);
	frustum.SetPerspective(DEGTORAD * 90.0f, DEGTORAD * 59.0f);
	frustum.SetViewPlaneDistances(0.1f, 300.0f);

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

	frustum.pos = trans->GetLocalPosition(); 	
	frustum.SetFront(trans->GetGlobalTransform().WorldZ().Normalized());
	frustum.SetUp(trans->GetGlobalTransform().WorldY().Normalized());


	return true;
}

void ComponentCamera::OnLoad(json_file * config)
{
	//Load Frustum Values
	frustum.pos.x = config->GetFloat("Frustum", 0.f, 0);
	frustum.pos.y = config->GetFloat("Frustum", 0.f, 1);
	frustum.pos.z = config->GetFloat("Frustum", 0.f, 2);

	frustum.front.x = config->GetFloat("Frustum", 0.f, 3);
	frustum.front.y = config->GetFloat("Frustum", 0.f, 4);
	frustum.front.z = config->GetFloat("Frustum", 1.f, 5);

	frustum.up.x = config->GetFloat("Frustum", 0.f, 6);
	frustum.up.y = config->GetFloat("Frustum", 1.f, 7);
	frustum.up.z = config->GetFloat("Frustum", 0.f, 8);

	frustum.nearPlaneDistance = config->GetFloat("Frustum", 0.1f, 9);
	frustum.farPlaneDistance = config->GetFloat("Frustum", 1000.f, 10);

	frustum.horizontalFov = config->GetFloat("Frustum", 1.f, 11);
	frustum.verticalFov = config->GetFloat("Frustum", 1.f, 12);
	
	//remember that we have to recalculate when modifying the projection
	//so:
	screen_resized = true;
}

void ComponentCamera::OnSave(json_file & config) const
{
	config.SetArrayFloat("Frustum", (float*)&frustum.pos.x, 13);
}



void ComponentCamera::DrawFrustum(Frustum & frustum, Color color)
{
	float3 frustum_vertices[8]; 
	frustum.GetCornerPoints(frustum_vertices);
	
	DebugDrawBox(frustum_vertices, color);
}

float ComponentCamera::GetNearPlaneDist() const
{
	return frustum.nearPlaneDistance;
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

bool ComponentCamera::IsInside(AABB & GO_bb)
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
	
	frustum.SetPerspective(frustum.horizontalFov, frustum.verticalFov);
	screen_resized = true;

}

void ComponentCamera::MoveForward(const float & speed)
{
	float3 vec = float3::zero;
	vec += frustum.Front() * speed;
	frustum.Translate(vec);
}

void ComponentCamera::MoveBackwards(const float & speed)
{
	float3 vec = float3::zero;
	vec -= frustum.Front() * speed;
	frustum.Translate(vec);
}

void ComponentCamera::MoveRight(const float & speed)
{
	float3 vec = float3::zero;
	vec += frustum.WorldRight() * speed;
	frustum.Translate(vec);
}

void ComponentCamera::MoveLeft(const float & speed)
{
	float3 vec = float3::zero;
	vec -= frustum.WorldRight() * speed;
	frustum.Translate(vec);
}

void ComponentCamera::MoveUp(const float & speed)
{
	float3 vec = float3::zero;
	vec += float3::unitY * speed;
	frustum.Translate(vec);
}

void ComponentCamera::MoveDown(const float & speed)
{
	float3 vec = float3::zero;
	vec -= float3::unitY * speed;
	frustum.Translate(vec);
}

void ComponentCamera::SetPosition(const float3 & pos)
{
	frustum.SetPos(pos);
}

void ComponentCamera::Orbit(const float3 & point, const float & motion_x, const float & motion_y)
{
	float3 vec = frustum.Pos() - point;

	Quat X(frustum.WorldRight(), motion_y);
	Quat Y(frustum.Up(), motion_x);

	vec = X.Transform(vec);
	vec = Y.Transform(vec);

	frustum.SetPos(vec + point);
}

void ComponentCamera::Rotate(const float & motion_x, const float & motion_y)
{
	Quat rotation_x = Quat::RotateY(motion_x);
	frustum.SetFront(rotation_x.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_x.Mul(frustum.Up()).Normalized());

	Quat rotation_y = Quat::RotateAxisAngle(frustum.WorldRight(), motion_y);
	frustum.SetFront(rotation_y.Mul(frustum.Front()).Normalized());
	frustum.SetUp(rotation_y.Mul(frustum.Up()).Normalized());
}

void ComponentCamera::Focus(const float3 & focus_center, const float & distance)
{
	float3 dir = frustum.Pos() - focus_center;
	frustum.SetPos(dir.Normalized() * distance);

	Look(focus_center);
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
