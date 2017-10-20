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
	frustum.farPlaneDistance = 1000.0f;
	//frustum.verticalFov =  * 60.0f;
	//SetAspectRatio(1.3f);
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update()
{
	return false;
}

void ComponentCamera::DrawFrustum()
{
	float3 frustum_vertices[8]; 
	frustum.GetCornerPoints(frustum_vertices);
	
	DebugDraw(frustum, Color(255,255,255,0));

}
