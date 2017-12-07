#include "ComponentBillboarding.h"
#include "ComponentDefs.h"
#include "GameObject.h"

ComponentBillboarding::ComponentBillboarding(GameObject * parent)
{
	this->parent = parent; 
	active = true; 
	reference = nullptr; 
	curr_relation = {0,0,0};
	show_set_window = false; 
	type = COMPONENT_BILLBOARDING;

	y_axis_locked = true; 
	x_axis_locked = false; 
}

ComponentBillboarding::~ComponentBillboarding()
{
}


bool ComponentBillboarding::GetShowInputWindow() const
{
	return show_set_window; 
}


void ComponentBillboarding::SetReference(GameObject * reference)
{
	this->reference = reference; 
}

GameObject * ComponentBillboarding::GetReference()
{
	return reference;
}

void ComponentBillboarding::SetShowInputWindow(bool new_set)
{
	show_set_window = new_set; 
}

bool ComponentBillboarding::Update()
{
	//if the gameobject reference has moved, we get the transform of the element with billboarding 
	ComponentTransform* trans = nullptr;

	if (reference != nullptr)
		trans = (ComponentTransform*)reference->GetComponent(COMPONENT_TRANSFORM);

	if (trans != nullptr && trans->IsPositionChanged() == true)
	{
		float3 new_x_axis = { 0,0,0 };
		float3 new_y_axis = { 0,0,0 };
		float3 new_z_axis = { 0,0,0 };

		float3 old_x_axis = GetComponentParent()->transform->GetGlobalTransform().WorldX();
		float3 old_y_axis = GetComponentParent()->transform->GetGlobalTransform().WorldY();
		float3 old_z_axis = GetComponentParent()->transform->GetGlobalTransform().WorldZ();

		//We set the Z axis at the direction of the reference
		float3 global_reference_pos = trans->GetGlobalPosition(); 
		float3 global_object_pos = GetComponentParent()->transform->GetGlobalPosition(); 

		float3 direction = global_reference_pos - global_object_pos; 

		direction.Normalize(); 

		new_z_axis = direction; 

		trans->SetPositionChanged(false); 

		//From that, we calculate the X new axis
		new_x_axis = new_z_axis.Perpendicular(); 
		new_x_axis *= -1; 

		//Then we get the Y new axis
		new_y_axis = new_x_axis.Cross(new_z_axis); 

		//We make calculations to know necessary rotation
		float cross = new_x_axis.x*old_x_axis.x + new_x_axis.y*old_x_axis.y + new_x_axis.z*old_x_axis.z;

		float angle = old_z_axis.AngleBetween(new_z_axis); 

		//We perfom the rotation
		GetComponentParent()->transform->SetLocalRotation({ 0, angle, 0});

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
