#include "ComponentBillboarding.h"
#include "ComponentDefs.h"
#include "GameObject.h"

ComponentBillboarding::ComponentBillboarding(GameObject * parent)
{
	this->parent = parent; 
	reference = nullptr; 
	curr_relation = {0,0,0};
	type = COMPONENT_BILLBOARDING;
}

ComponentBillboarding::~ComponentBillboarding()
{
}

void ComponentBillboarding::AttachObject(GameObject* new_reference)
{
	reference = new_reference; 

	ComponentTransform* ref_trans = (ComponentTransform*)reference->GetComponent(COMPONENT_TRANSFORM);
	ComponentTransform* this_trans = (ComponentTransform*)GetComponentParent()->GetComponent(COMPONENT_TRANSFORM);

	if (ref_trans != nullptr)
	{
		curr_relation = ref_trans->GetGlobalPosition() - this_trans->GetGlobalPosition(); 
	}
		
}

bool ComponentBillboarding::Update()
{

	if (reference != nullptr)
	{

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
