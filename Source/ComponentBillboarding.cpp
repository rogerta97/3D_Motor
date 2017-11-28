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

void ComponentBillboarding::AttachObject(GameObject* new_reference)
{

		
}

bool ComponentBillboarding::GetShowInputWindow() const
{
	return show_set_window; 
}

void ComponentBillboarding::SetShowInputWindow(bool new_set)
{
	show_set_window = new_set; 
}

bool ComponentBillboarding::Update()
{

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
