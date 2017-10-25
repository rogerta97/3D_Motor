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
				GameObject* curr_go_drawed = App->scene_intro->GetGameObject(i);
		
				if (curr_go_drawed->GetParent() != nullptr)			
					continue;
									
				if (!curr_go_drawed->IsChildEmpty())
				{
					if (ImGui::TreeNode(curr_go_drawed->GetName()))
					{
						for (int j = 0; j < curr_go_drawed->GetNumChilds(); j++)
						{
							//string name(curr_go_drawed->GetChild(j)->GetName()); 

							//name += " ";
							//name += to_string(j + 1); 
							//curr_go_drawed->GetChild(j)->SetName(name.c_str()); 

							if (ImGui::MenuItem(curr_go_drawed->GetChild(j)->GetName()))
							{
								App->scene_intro->SetCurrentGO(curr_go_drawed->GetChild(j)->GetID());
							}
						}

						ImGui::TreePop();
					}

				}
				else
				{
					if (ImGui::MenuItem(curr_go_drawed->GetName()))
					{
						App->scene_intro->SetCurrentGO(i);
					}
				}

			}
		}

		ImGui::End();
	}
	


	return true; 
}
