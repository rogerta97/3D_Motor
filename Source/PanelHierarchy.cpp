#include "PanelHierarchy.h"
#include "Application.h"
#include "GameObject.h"

bool PanelHierarchy::Draw()
{
	if (!show_hierarchy)
		return false; 

	if (ImGui::Begin("Hierarchy"))
	{
		if (App->scene_intro->IsListEmpty() == false)
		{
			for (int i = 0; i < App->scene_intro->GetList().size(); i++)
			{				
				GameObject* go_to_draw = App->scene_intro->GetGameObject(i); 

				if (go_to_draw->GetParent() == nullptr)
					DrawNode(go_to_draw); 
			}
		}
		ImGui::End();
	}
	
	return true; 
}

void PanelHierarchy::DrawNode(GameObject * go)
{
	if (!go->IsChildEmpty())
	{
		if (ImGui::TreeNode(go->GetName()))
		{
			if (!inner_child_selected)
				App->scene_intro->SetCurrentGO(go->GetID());

			for (int j = 0; j < go->GetNumChilds(); j++)
			{
				GameObject* curr_child = go->GetChild(j);

				if (curr_child->GetNumChilds() > 0)
				{
					DrawNode(curr_child);
				}
				else
				{
					if (ImGui::MenuItem(curr_child->GetName()))
					{
						App->scene_intro->SetCurrentGO(go->GetChild(j)->GetID());
						inner_child_selected = true;
						outter_child_selected = false; 
					}
				}

			}

			ImGui::TreePop();
		}
		else if (outter_child_selected = false)
		{
			App->scene_intro->SetCurrentGO(go->GetID());
			inner_child_selected = false;
		}
			
	}
	else
	{
		if (ImGui::MenuItem(go->GetName()))
		{
			App->scene_intro->SetCurrentGO(go->GetID());
			outter_child_selected = true; 
		}
	}
}
