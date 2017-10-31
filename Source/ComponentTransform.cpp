#include "ComponentTransform.h"
#include "OpenGL.h"
#include "GameObject.h"
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

ComponentTransform::ComponentTransform(GameObject* _parent)
{
	parent = _parent; 
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
	SetRotation(rot.identity);
	SetScale({ 1,1,1 }); 
}

bool ComponentTransform::Update()
{


	return false;
}



void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetComponentParent()->IsStatic() == false)
		position = _position;

}

void ComponentTransform::SetRotation(const float3& _rotation)
{
	Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
	rotation =  mod;
}
void ComponentTransform::SetRotation(const Quat& _rotation)
{
	if (GetComponentParent()->IsStatic() == false)
		rotation = _rotation;
	
}

void ComponentTransform::SetScale(const float3 & scale)
{
	if (GetComponentParent()->IsStatic() == false)
		this->scale = scale;

}