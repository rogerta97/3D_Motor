#include "ComponentTransform.h"
#include "ComponentMeshRenderer.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Application.h"
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

float3 ComponentTransform::GetGlobalPosition() const
{

	return position;
}

Quat ComponentTransform::GetGlobalRotation() const
{
	return rotation;
}

float3 ComponentTransform::GetGlobalScale() const
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

void ComponentTransform::SetLocalTransform(float4x4 new_transform)
{
	local_transform = new_transform;
}

float4x4 ComponentTransform::GetLocalTransform()
{
	return local_transform;
}

void ComponentTransform::SetIdentityTransform()
{
	SetLocalPosition({ 0,0,0 });
	Quat rot; 
	SetLocalRotation({ 0.0f, 0.0f, 0.0f });
	SetLocalScale({ 1,1,1 });
}

void ComponentTransform::SetGlobalTransform(float4x4 new_transform)
{
}

float4x4 ComponentTransform::GetGlobalTransform()
{
	return float4x4();
}

bool ComponentTransform::IsModified() const
{
	return transform_modified;
}

void ComponentTransform::SetModified(bool n_value)
{
	transform_modified = n_value; 
}

void ComponentTransform::UpdateTransform()
{

	local_transform.Set(float4x4::FromTRS(position, rotation, scale)); 

	if (GetComponentParent()->GetParent() == nullptr)
		global_transform = local_transform; 

	else
	{
		GameObject* parent_go = GetComponentParent()->GetParent(); 

		while (parent_go != nullptr)
		{
			ComponentTransform* p_trans = (ComponentTransform*)parent_go->GetComponent(COMPONENT_TRANSFORM);

			global_transform = p_trans->GetLocalTransform().Mul(local_transform);
			parent_go = parent_go->GetParent(); 
		}
	}

}

bool ComponentTransform::Update()
{


	return false;
}



void ComponentTransform::SetLocalPosition(const float3 & _position)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		position = _position;	
		transform_modified = true; 

		UpdateTransform();

		if (GetComponentParent()->GetNumChilds() > 0)
		{
			for (int i = 0; i < GetComponentParent()->GetNumChilds(); i++)
			{
				ComponentTransform* child_trans = (ComponentTransform*)GetComponentParent()->GetChild(i)->GetComponent(COMPONENT_TRANSFORM); 
				child_trans->SetLocalPosition(position); 
			}
		}
	}
		
}

void ComponentTransform::SetLocalRotation(const float3& _rotation)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;

		transform_modified = true;

		UpdateTransform(); 

		if (GetComponentParent()->GetNumChilds() > 0)
		{
			for (int i = 0; i < GetComponentParent()->GetNumChilds(); i++)
			{
				ComponentTransform* child_trans = (ComponentTransform*)GetComponentParent()->GetChild(i)->GetComponent(COMPONENT_TRANSFORM);
				child_trans->SetLocalRotation(position);
			}
		}

	}
}


void ComponentTransform::SetLocalScale(const float3 & _scale)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		scale = _scale;

		transform_modified = true;

		UpdateTransform();

		if (GetComponentParent()->GetNumChilds() > 0)
		{
			for (int i = 0; i < GetComponentParent()->GetNumChilds(); i++)
			{
				ComponentTransform* child_trans = (ComponentTransform*)GetComponentParent()->GetChild(i)->GetComponent(COMPONENT_TRANSFORM);
				child_trans->SetLocalScale(position);
			}
		}
	}
		

}