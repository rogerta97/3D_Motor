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

				if (!curr_go_drawed->IsChildEmpty())
				{

					if (ImGui::TreeNode(curr_go_drawed->GetName()))
					{
						for (int j = 0; j < curr_go_drawed->GetNumChilds(); j++)
						{
							string name(curr_go_drawed->GetChild(j)->GetName()); 

							name += to_string(j + 1); 

							if (ImGui::MenuItem(name.c_str()))
							{
								LOG("current %d", j);
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
