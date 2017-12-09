#include "ComponentBillboarding.h"
#include "ComponentDefs.h"
#include "GameObject.h"

ComponentBillboarding::ComponentBillboarding(GameObject * parent)
{
	gameobject = parent; 
	active = true; 
	bill_active = false; 
	reference = nullptr; 
	type = COMPONENT_BILLBOARDING;

	y_axis_locked = true; 
	x_axis_locked = false; 

	angle_x = 0; 
	angle_y = 0; 
	angle_z = 0; 
}

ComponentBillboarding::~ComponentBillboarding()
{
}





void ComponentBillboarding::SetReference(ComponentCamera * reference)
{
	this->reference = reference; 
}

ComponentCamera * ComponentBillboarding::GetReference()
{
	return reference;
}

bool ComponentBillboarding::Update()
{
	//if the gameobject reference has moved, we get the transform of the element with billboarding 
	float3 reference_position;

	if (reference != nullptr)
	{
		reference_position = reference->frustum.pos;

		if (reference_position.x != last_ref_pos.x || reference_position.y != last_ref_pos.y || reference_position.z != last_ref_pos.z)
			ref_position_changed = true;
	}
		
	if (ref_position_changed == true)
	{
		last_ref_pos = reference_position; 

		float3 new_x_axis = { 0,0,0 };
		float3 new_y_axis = { 0,0,0 };
		float3 new_z_axis = { 0,0,0 };

		float3 old_x_axis = GetComponentParent()->transform->GetGlobalTransform().WorldX();
		float3 old_y_axis = GetComponentParent()->transform->GetGlobalTransform().WorldY();
		float3 old_z_axis = GetComponentParent()->transform->GetGlobalTransform().WorldZ();

		//We set the Z axis at the direction of the reference
		float3 global_object_pos = GetComponentParent()->transform->GetGlobalPosition(); 

		float3 direction = reference_position - global_object_pos;

		direction.Normalize(); 

		new_z_axis = direction; 

		ref_position_changed = false;

		//From that, we calculate the X new axis
		new_x_axis = new_z_axis.Perpendicular(); 
		new_x_axis *= -1; 

		//Then we get the Y new axis
		new_y_axis = new_x_axis.Cross(new_z_axis); 
		new_y_axis *= -1;

		//We make calculations to know necessary rotation
		
		float cross = new_x_axis.x*old_x_axis.x + new_x_axis.y*old_x_axis.y + new_x_axis.z*old_x_axis.z;
		
		float prev_angle_x = old_x_axis.AngleBetween(new_x_axis);
		float prev_angle_z = old_z_axis.AngleBetween(new_z_axis);

		if (new_z_axis.y > old_z_axis.y)
		{
			prev_angle_z = 360 - (prev_angle_z*RADTODEG);
			prev_angle_z *= DEGTORAD;
		}
		//if (new_z_axis.y > old_z_axis.y)
		//{
		//	angle_y = 360 - (angle_y*RADTODEG);
		//	angle_y *= DEGTORAD;
		//}

		//Add it to the current angles 
		angle_x += prev_angle_x; 
		angle_z += prev_angle_z; 
					
		//We perfom the rotation
		GetComponentParent()->transform->SetLocalRotation({ angle_z, 0, 0});

	}

	return false;
}

void ComponentBillboarding::OnLoad(json_file * config)
{
}

void ComponentBillboarding::OnSave(json_file & config) const
{
}

void ComponentBillboarding::Delete()
{
}
