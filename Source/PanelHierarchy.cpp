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
	uint flags = 0;// ImGuiTreeNodeFlags_OpenOnArrow;

	if (go->GetNumChilds() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (go == App->scene_intro->GetCurrentGO())
	{
		flags |= ImGuiTreeNodeFlags_Selected;
		open_selected = false;
	}

	if (open_selected == true && App->scene_intro->GetCurrentGO()->IsChild(go) == true)
		ImGui::SetNextTreeNodeOpen(true);

	if (ImGui::TreeNodeEx(go->GetName(), flags))
	{
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0)) {
				App->scene_intro->SetCurrentGO(go->GetID());
			}

		/*	if (ImGui::IsMouseDoubleClicked(0))
			{
				float radius = go->global_bbox.MinimalEnclosingSphere().r;
				App->camera->CenterOn(go->GetGlobalPosition(), std::fmaxf(radius, 5.0f) * 2.0f);
			}*/
		}

		if (ImGui::IsItemClicked(0)) {
			App->scene_intro->SetCurrentGO(go->GetID());
		}


		for (vector<GameObject*>::const_iterator it = go->child_list.begin(); it != go->child_list.end(); ++it)
			DrawNode(*it);

		ImGui::TreePop();
	}
	else
	{
		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0)) {
				App->scene_intro->SetCurrentGO(go->GetID());
			}

			/*	if (ImGui::IsMouseDoubleClicked(0))
			{
			float radius = go->global_bbox.MinimalEnclosingSphere().r;
			App->camera->CenterOn(go->GetGlobalPosition(), std::fmaxf(radius, 5.0f) * 2.0f);
			}*/
		}

	}
}
