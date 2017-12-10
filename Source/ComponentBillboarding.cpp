#include "ComponentBillboarding.h"
#include "ComponentDefs.h"
#include "Application.h"
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
		if (!x_axis_locked)
		{
			new_x_axis = new_z_axis.Perpendicular();

			if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				new_x_axis *= -1;
		}
		else
			new_x_axis = old_x_axis; 


		//Then we get the Y new axis

		if (!y_axis_locked)
		{
			new_y_axis = new_x_axis.Cross(new_z_axis);
			new_y_axis *= -1;
		}
		else
			new_y_axis = old_y_axis; 
		

		//We make calculations to know necessary rotation	
		
		///First we project our current axis into X and Y planes 
		float3 old_x_projection = { old_z_axis.x, 0, old_z_axis.z }; 
		float3 old_y_projection = { old_z_axis.z, old_z_axis.y, 0 }; 

		///We project the X of the triangle formed with the angle for knowing the horizontal rotation
		float3 new_x_projection = { new_z_axis.x, 0, new_z_axis.z };
		float3 new_y_projection = { new_z_axis.z, new_z_axis.y, 0 };

		new_y_projection.x = new_x_projection.z; 

		///We project the Y of the triangle formed with the angle for knowing the vertical rotation
		float increment_angle_x = old_x_projection.AngleBetween(new_x_projection);
		float increment_angle_y = old_y_projection.AngleBetween(new_y_projection);

		if (new_z_axis.y > old_z_axis.y)
		{
			increment_angle_y = 360 - (increment_angle_y*RADTODEG);
			increment_angle_y *= DEGTORAD;
		}

		//if (new_z_axis.y > old_z_axis.y)
		//{
		//	angle_y = 360 - (angle_y*RADTODEG);
		//	angle_y *= DEGTORAD;
		//}

		//Add it to the current angles 
		angle_x += increment_angle_y;
		angle_y += increment_angle_x;
					
		//We perfom the rotation

		if(!y_axis_locked)
			GetComponentParent()->transform->SetLocalRotation({ angle_x, 0, 0 });

		if(!x_axis_locked)
			GetComponentParent()->transform->SetLocalRotation({ 0, angle_y, 0 });

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
