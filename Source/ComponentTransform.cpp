#include "ComponentTransform.h"
#include "ComponentMeshRenderer.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Application.h"
#include "MathGeoLib\MathGeoLib.h"
#include "ImGuizmo.h"
#include "JSON.h"

float3 ComponentTransform::GetLocalPosition()const
{
	return transform.position;
}

Quat ComponentTransform::GetLocalRotation()const
{
	return transform.rotation;
}

float3 ComponentTransform::GetLocalScale()const
{

	return transform.scale;
}

float3 ComponentTransform::GetGlobalPosition()
{

	return global_transform.position;
}

Quat ComponentTransform::GetGlobalRotation()
{

	return global_transform.rotation;
}

float3 ComponentTransform::GetGlobalScale()
{

	return global_transform.scale;
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

void ComponentTransform::SetLocalTransformMat(float4x4 new_transform)
{
	local_transform_mat = new_transform;
}

float4x4 ComponentTransform::GetLocalTransform()
{
	return local_transform_mat;
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
	global_transform_mat = new_transform; 
}

float4x4 ComponentTransform::GetGlobalTransform()
{

	if (GetComponentParent()->GetParent() == nullptr)
		global_transform_mat = local_transform_mat;

	else
	{
		GameObject* parent_go = GetComponentParent()->GetParent();

		while (parent_go != nullptr)
		{
			ComponentTransform* parent_trans = (ComponentTransform*)parent_go->GetComponent(COMPONENT_TRANSFORM);
			
			global_transform_mat = parent_trans->local_transform_mat.Mul(local_transform_mat);

			parent_go = parent_go->parent;
		}
	}

	global_transform_mat.Decompose(global_transform.position, global_transform.rotation, global_transform.scale);

	return global_transform_mat;
}

bool ComponentTransform::IsModified() const
{
	return transform_modified;
}

void ComponentTransform::SetModified(bool modified)
{
	transform_modified = modified;
}

void ComponentTransform::UpdateTransform(GameObject* curr_go)
{
		local_transform_mat.Set(float4x4::FromTRS(transform.position, transform.rotation, transform.scale));
}

bool ComponentTransform::Update()
{

	if (IsModified())
	{
		GetComponentParent()->RecursiveAdaptBoundingBox(GetGlobalTransform(), GetComponentParent());
		SetModified(false);
	}

	return true;
}

void ComponentTransform::OnLoad(json_file * config)
{
	transform.position = config->GetFloat3("Position", float3::zero);
	transform.rotation = config->GetQuat("Position", Quat::identity);
	transform.scale = config->GetFloat3("Scale", float3::zero);

	transform_modified = true;
}

void ComponentTransform::OnSave(json_file & config) const
{
	config.SetFloat3("Position", transform.position);
	config.SetQuaternion("Rotation", transform.rotation);
	config.SetFloat3("Scale", transform.scale);
}

void ComponentTransform::DeleteComponent()
{
}


void ComponentTransform::SetLocalPosition(const float3 & _position)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		transform.position = _position;
		UpdateTransform(GetComponentParent()); 
		transform_modified = true; 
	}	
}

void ComponentTransform::SetLocalRotation(const float3& _rotation)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		transform.rotation = mod;
		
		UpdateTransform(GetComponentParent());
		transform_modified = true;
	}
}


void ComponentTransform::SetLocalScale(const float3 & _scale)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		transform.scale = _scale;

		UpdateTransform(GetComponentParent());
		transform_modified = true;
	}
}