#include "ComponentTransform.h"
#include "ComponentMeshRenderer.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

float3 ComponentTransform::GetLocalPosition()const
{
	return position;
}

Quat ComponentTransform::GetLocalRotation()const
{
	return rotation;
}

float3 ComponentTransform::GetLocalScale()const
{
	return scale;
}

ComponentTransform::ComponentTransform(GameObject* _parent)
{
	parent = _parent; 
	active = true; 
	type = COMPONENT_TRANSFORM; 
	transform_modified = false; 
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetTransformMatrix()
{
	
}

const float* ComponentTransform::GetLocalTransform()
{
	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);
	return transform_matrix.Transposed().ptr();
}

int ComponentTransform::GetTransformID()const
{
	return transform_id;
}

float4x4 ComponentTransform::GetTransformMatrix()
{
	transform_matrix = transform_matrix.FromTRS(position, rotation, scale);
	return transform_matrix;
}

void ComponentTransform::SetIdentityTransform()
{
	SetPosition({ 0,0,0 }); 
	Quat rot; 
	SetRotation({ 0.0f, 0.0f, 0.0f });
	SetScale({ 1,1,1 }); 
}

bool ComponentTransform::IsModified() const
{
	return transform_modified;
}

void ComponentTransform::SetModified(bool n_value)
{
	transform_modified = n_value; 
}

bool ComponentTransform::Update()
{


	return false;
}



void ComponentTransform::SetPosition(const float3 & _position)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		position = _position;	
		transform_modified = true; 
	}
		
}

void ComponentTransform::SetRotation(const float3& _rotation)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;

		transform_modified = true;

	}
}


void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		scale = _scale;

		transform_modified = true;
	}
		

}