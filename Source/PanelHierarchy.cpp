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
					}
				}

			}

			ImGui::TreePop();
		}
	}
}
