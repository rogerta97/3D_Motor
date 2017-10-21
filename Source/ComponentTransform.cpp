#include "ComponentTransform.h"
#include "OpenGL.h"
#include "MathGeoLib\MathGeoLib.h"

float3 ComponentTransform::GetLocalPosition()
{
	return position;
}

Quat ComponentTransform::GetLocalRotation()
{
	return rotation;
}

float3 ComponentTransform::GetLocalScale()
{
	return scale;
}

ComponentTransform::ComponentTransform()
{
	active = true; 	
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetTransformMatrix()
{
	
}

const float* ComponentTransform::GetGlobalTransform()
{
	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);
	return transform_matrix.Transposed().ptr();
}

int ComponentTransform::GetTransformID()
{
	return transform_id;
}

void ComponentTransform::SetIdentityTransform()
{
	SetPosition({ 0,0,0 }); 
	Quat rot; 
	rot.identity; 
	SetRotation(rot); 
	SetScale({ 1,1,1 }); 
}

bool ComponentTransform::Update()
{


	return false;
}



void ComponentTransform::SetPosition(const float3 & _position)
{
	position = _position;
}

void ComponentTransform::SetRotation(const Quat & rotation)
{
	this->rotation = rotation;
}

void ComponentTransform::SetScale(const float3 & scale)
{
	this->scale = scale;
}