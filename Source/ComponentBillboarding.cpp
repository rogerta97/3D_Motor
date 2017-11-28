#include "ComponentBillboarding.h"
#include "ComponentDefs.h"
#include "GameObject.h"

ComponentBillboarding::ComponentBillboarding(GameObject * parent)
{
	this->parent = parent; 
	reference = nullptr; 
	curr_relation = {0,0,0};
	show_set_window = false; 
	type = COMPONENT_BILLBOARDING;
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
	ComponentTransform* trans = (ComponentTransform*)reference->GetComponent(COMPONENT_TRANSFORM); 

	if (trans != nullptr && trans->IsModified() == true)
	{

	}
	//We equal the Z axis at the direction of the camera

	//From that, we calculate the X new axis

	//Then we get the Y new axis

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
