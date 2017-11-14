#include "ComponentTransform.h"
#include "ComponentMeshRenderer.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Application.h"
#include "MathGeoLib\MathGeoLib.h"
#include "ImGuizmo.h"

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

float3 ComponentTransform::GetGlobalPosition()
{

	return global_position;
}

Quat ComponentTransform::GetGlobalRotation()
{

	return global_rotation;
}

float3 ComponentTransform::GetGlobalScale()
{

	return global_scale;
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
	local_transform = new_transform; 
}

float4x4 ComponentTransform::GetGlobalTransform()
{

	if (GetComponentParent()->GetParent() == nullptr)
		global_transform = local_transform; 

	else
	{
		GameObject* parent_go = GetComponentParent()->GetParent();

		while (parent_go != nullptr)
		{
			ComponentTransform* parent_trans = (ComponentTransform*)parent_go->GetComponent(COMPONENT_TRANSFORM);

			global_transform = parent_trans->local_transform.Mul(local_transform);

			parent_go = parent_go->parent;
		}
	}

	global_transform.Decompose(global_position, global_rotation, global_scale);

	return global_transform;
}

bool ComponentTransform::IsModified() const
{
	return transform_modified;
}

void ComponentTransform::SetModified(bool n_value)
{
	transform_modified = n_value; 
}

void ComponentTransform::UpdateTransform(GameObject* curr_go)
{
		local_transform.Set(float4x4::FromTRS(position, rotation, scale));

		ComponentMeshRenderer* mr = (ComponentMeshRenderer*)curr_go->GetComponent(COMPONENT_MESH_RENDERER);

		if (mr != nullptr)
			mr->AdaptBoundingBox(GetGlobalTransform());

		for(int i = 0; i < curr_go->child_list.size(); i++)
		{
			mr = (ComponentMeshRenderer*)curr_go->child_list[i]->GetComponent(COMPONENT_MESH_RENDERER);
			ComponentTransform* trans = (ComponentTransform*)curr_go->child_list[i]->GetComponent(COMPONENT_TRANSFORM);
			
			trans->UpdateTransform(curr_go->child_list[i]);
	
		}



		

		
			
			
		
		//global_transform = local_transform;

		//GameObject* parent_go = GetComponentParent()->GetParent();

		//while (parent_go != nullptr)
		//{
		//	ComponentTransform* parent_trans = (ComponentTransform*)parent_go->GetComponent(COMPONENT_TRANSFORM);

		//	global_transform = parent_trans->local_transform.Mul(local_transform);

		//	parent_go = parent_go->parent;
		//}

	//global_transform.Decompose(global_position, global_rotation, global_scale);
	
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
		UpdateTransform(GetComponentParent()); 
		transform_modified = true; 
	}	
}

void ComponentTransform::SetLocalRotation(const float3& _rotation)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;
		
		UpdateTransform(GetComponentParent());
		transform_modified = true;
	}
}


void ComponentTransform::SetLocalScale(const float3 & _scale)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		scale = _scale;

		UpdateTransform(GetComponentParent());
		transform_modified = true;
	}
}