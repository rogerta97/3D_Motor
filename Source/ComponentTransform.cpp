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
	if (GetComponentParent()->IsStatic() == false)
	{
		Quat mod = Quat::FromEulerXYZ(_rotation.x, _rotation.y, _rotation.z);
		rotation = mod;

		ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponentParent()->GetComponent(COMPONENT_MESH_RENDERER);

		if (mr != nullptr)
		{

			int min[3]; 
			int max[3];

			mr->bounding_box.SetNegativeInfinity(); 
			mr->bounding_box.Enclose(mr->vertices, mr->num_vertices); 
	/*		mr->bounding_box.ExtremePointsAlongAABB(mr->vertices, mr->num_vertices, min[0], max[0], min[1], max[1], min[2], max[2]); 

			AABB n_box(vec(mr->vertices[min[0]].x, mr->vertices[min[1]].y, mr->vertices[min[2]].z), vec(mr->vertices[max[0]].x, mr->vertices[max[1]].y, mr->vertices[max[2]].z));
			mr->bounding_box = n_box;*/
		}
	}
}


void ComponentTransform::SetScale(const float3 & _scale)
{
	if (GetComponentParent()->IsStatic() == false)
	{
		
		ComponentMeshRenderer* mr = (ComponentMeshRenderer*)GetComponentParent()->GetComponent(COMPONENT_MESH_RENDERER);

		if (mr != nullptr)
		{
			vec temp_scale = _scale;
			vec center_point = mr->bounding_box.CenterPoint(); 

			mr->bounding_box = AABB(vec(-1, -1, -1), vec(1, 1, 1)); 
			mr->bounding_box.Scale(center_point, temp_scale);
		}

		scale = _scale;
	}
		

}