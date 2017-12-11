#include "ComponentBillboarding.h"
#include "ComponentDefs.h"
#include "Application.h"
#include "GameObject.h"

ComponentBillboarding::ComponentBillboarding(GameObject * parent, Particle* particle_parent)
{
	gameobject = parent; 
	active = true; 
	bill_active = false; 
	reference = nullptr; 
	type = COMPONENT_BILLBOARDING;
	this->particle_parent = particle_parent;

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

void ComponentBillboarding::LockY()
{
	y_axis_locked = true; 
}

void ComponentBillboarding::LockX()
{
	x_axis_locked = true;
}

void ComponentBillboarding::UnLockY()
{
	y_axis_locked = false;
}

void ComponentBillboarding::UnLockX()
{
	x_axis_locked = false;
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
		//We equal the position of the reference
		last_ref_pos = reference_position;

		//This is were new axis will be alocated
		float3 new_x_axis;
		float3 new_y_axis;

		//We calculate the new z_axis
		float3 new_z_axis = reference_position.Normalized();

		//From there we calculate the X axis 
		new_x_axis = new_z_axis.Perpendicular();
		new_x_axis *= -1;	

		//From those we get the Y axis
		new_y_axis = new_x_axis.Cross(new_z_axis);
		new_y_axis *= -1;

		if (!x_axis_locked)  //Rotate arround X axis (vertical)
		{
			float3 x_proj = { new_z_axis.x, 0, new_z_axis.z };
	

			float increment_angle_y = x_proj.AngleBetween({0,0,1});

			if (new_z_axis.x < 0)
				increment_angle_y *= -1; 

			if (particle_parent != nullptr)
			{
				particle_parent->components.particle_transform->SetLocalRotation({ 0, increment_angle_y, 0 }); 
			}
			else
				GetComponentParent()->transform->SetLocalRotation({ 0, increment_angle_y, 0 });
		}

		if (!y_axis_locked) //Rotate arround Y axis (horizontal)
		{
			float3 y_proj = { 0, new_z_axis.y, new_z_axis.z };

			float increment_angle_x = y_proj.AngleBetween({ 0,0,1 });

			if (new_z_axis.y > 0)
				increment_angle_x *= -1;

			if (particle_parent != nullptr)
			{
				particle_parent->components.particle_transform->SetLocalRotation({ increment_angle_x, 0, 0 });
			}
			else
				GetComponentParent()->transform->SetLocalRotation({ increment_angle_x, 0, 0 });
		}
		ref_position_changed = false;

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
