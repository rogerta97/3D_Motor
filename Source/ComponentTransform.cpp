#include "ComponentTransform.h"
#include "ComponentMeshRenderer.h"
#include "ComponentBillboarding.h"
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
	gameobject = _parent; 
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

bool ComponentTransform::IsPositionChanged() const
{
	return position_modified;
}

void ComponentTransform::SetPositionChanged(bool new_set)
{
	position_modified = new_set; 
}

void ComponentTransform::UpdateTransform()
{
		local_transform_mat.Set(float4x4::FromTRS(transform.position, transform.rotation, transform.scale));
}

float3 ComponentTransform::LocalX()
{
	float3 ret = local_transform_mat.WorldX(); 

	local_transform_mat.TransformPos(ret); 

	return ret;
}

float3 ComponentTransform::LocalY()
{
	float3 ret = local_transform_mat.WorldY();

	local_transform_mat.TransformPos(ret);

	return ret;
}

float3 ComponentTransform::LocalZ()
{

	float3 ret = local_transform_mat.WorldZ();

	local_transform_mat.TransformPos(ret);

	return ret;
}

void ComponentTransform::DrawAxis()
{
	LineSegment axis[3]; 

	//X
	axis[0].a = transform.position;
	axis[0].b = transform.position + LocalX().Normalized();

	//Y
	axis[1].a = transform.position;
	axis[1].b = transform.position + LocalY().Normalized();

	//Z
	axis[2].a = transform.position;
	axis[2].b = transform.position + LocalZ().Normalized();

	//Draw
	DebugDraw(axis[0], Red, true, float4x4::identity, 5.0f); 
	DebugDraw(axis[1], Green, true, float4x4::identity, 5.0f);
	DebugDraw(axis[2], Blue, true, float4x4::identity, 5.0f);
	
}


void ComponentTransform::Serialize(json_file * file)
{
	file->AddSectionToArray("Components");
	file->MoveToSectionFromArray("Components", file->GetArraySize("Components") - 1);
	
	file->SetInt("type", GetComponentType());
	file->SetInt("owner", GetComponentParent()->unique_id);
	file->SetFloat3("position", transform.position);
	file->SetFloat3("scale", transform.scale);
	file->SetQuaternion("rotation", transform.rotation);
}

void ComponentTransform::Serialize(json_file file)
{
	file.AddSectionToArray("Components");
	file.MoveToSectionFromArray("Components", file.GetArraySize("Components") - 1);
		
	file.SetInt("type", GetComponentType());
	file.SetInt("owner", GetComponentParent()->unique_id);
	file.SetFloat3("position", transform.position);
	file.SetFloat3("scale", transform.scale);
	file.SetQuaternion("rotation", transform.rotation);
}

void ComponentTransform::SerializeLoad(json_file* file)
{
	 transform.position = file->GetFloat3("position");
	 transform.rotation = file->GetQuat("rotation");
	 transform.scale = file->GetFloat3("scale");


}

void ComponentTransform::SerializeSave(json_file* file)
{
	file->SetFloat3("position", transform.position);
	file->SetQuaternion("rotation", transform.rotation);
	file->SetFloat3("scale", transform.scale);
}

bool ComponentTransform::Update()
{
	float3 testpos[3]; 

	testpos[0] = GetLocalTransform().WorldX(); 
	testpos[1] = GetLocalTransform().WorldY();
	testpos[2] = GetLocalTransform().WorldZ();

	if (IsModified())
	{
		GetComponentParent()->RecursiveAdaptBoundingBox(GetGlobalTransform(), GetComponentParent());		
		SetModified(false);
	}

	DrawAxis();

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
	ComponentMeshRenderer* mr;

	if (gameobject != nullptr)
	{
		mr = (ComponentMeshRenderer*)GetComponentParent()->GetComponent(COMPONENT_MESH_RENDERER);

		if (GetComponentParent()->IsStatic() == true)
			return; 
	}
		
	transform.position = _position;
	UpdateTransform(); 
	transform_modified = true; 
	position_modified = true; 
		
}

void ComponentTransform::SetLocalRotation(const float3& _rotation)
{
	if (GetComponentParent() == nullptr) //This means it is a particle 
	{
	
		Quat mod = Quat::FromEulerXYZ(_rotation.x  * DEGTORAD, _rotation.y  * DEGTORAD, _rotation.z  * DEGTORAD);
		transform.rotation = transform.rotation*mod;

		transform_modified = true;
		UpdateTransform();
	}

	else if (GetComponentParent()->IsStatic() == false)
	{		
		Quat mod = Quat::FromEulerXYZ(_rotation.x * DEGTORAD, _rotation.y * DEGTORAD, _rotation.z*DEGTORAD);
		transform.rotation = transform.rotation* mod;
		
		transform_modified = true;
		UpdateTransform();
	
	}

}


void ComponentTransform::SetLocalScale(const float3 & _scale)
{
	if (GetComponentParent() == nullptr)
	{
		transform.scale = _scale;
		transform_modified = true;
		UpdateTransform();
	}
	else if (GetComponentParent()->IsStatic() == false)
	{
		transform.scale = _scale;
		transform_modified = true;
		UpdateTransform();
	}
}