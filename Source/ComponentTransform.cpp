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
	{
		float3 translation = _position - position;
		position = _position;

		ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponentParent()->GetComponent(COMPONENT_MESH_RENDERER); 

		if (mr != nullptr)
		{					
			mr->bounding_box.Translate(translation);
		}
	
	}
		
		

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

void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		scale = _scale;

		ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponentParent()->GetComponent(COMPONENT_MESH_RENDERER);

		if (mr != nullptr)
		{
			vec temp_scale = vec(scale[0], scale[1], scale[2]);
			vec center_point = mr->bounding_box.CenterPoint(); 
			mr->bounding_box.Scale(center_point, temp_scale);
		}
	}
		

}