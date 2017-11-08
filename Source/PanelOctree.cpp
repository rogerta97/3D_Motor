#include "PanelOctree.h"
#include "Octree.h"
#include "Application.h"

void PanelOctree::Start()
{
	oct_active = false; 

	if (App->scene_intro->GetOctree() == nullptr)
	{
		min_point[0] = min_point[1] = min_point[2] = 0.0f;
		max_point[0] = max_point[1] = max_point[2] = 0.0f;
	}
	else
	{
		OctreeNode* root_node = App->scene_intro->GetOctree()->GetRootNode(); 

		min_point[0] = root_node->box.minPoint.x;
		min_point[1] = root_node->box.minPoint.y;
		min_point[2] = root_node->box.minPoint.z;

		max_point[0] = root_node->box.maxPoint.x;
		max_point[1] = root_node->box.maxPoint.y;
		max_point[2] = root_node->box.maxPoint.z;
	}


}

void PanelOctree::DrawPanel()
{
	if (ImGui::Begin("Octree"))
	{
		ImGui::Separator(); 
		ImGui::TextWrapped("Octree will optimize the searching for STATIC gameobjects. When an object is inserted its bounding box will be displayed in red, while octree will appear green"); 
		ImGui::Separator(); 

		ImGui::Checkbox("Active", &oct_active);

		if (ImGui::TreeNode("Create New Octree"))
		{
			ImGui::InputFloat3("Min Point", &min_point[0], 2);
			ImGui::InputFloat3("Max Point", &max_point[0], 2);

			if (ImGui::Button("Create!"))
			{
				Octree* oct = App->scene_intro->GetOctree(); 
				oct = new Octree(); 
				AABB limits(min_point, max_point);
				oct->Create(limits);				
			}

			ImGui::TreePop(); 
		}

		ImGui::Text("Min Point: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "(%.2f, %.2f, %.2f)", min_point.x, min_point.y, min_point.z); 

		ImGui::Text("Max Point: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "(%.2f, %.2f, %.2f)", max_point.x, max_point.y, max_point.z);

		if(App->scene_intro->GetOctree() != nullptr)
			App->scene_intro->GetOctree()->SetActive(oct_active); 
 
		ImGui::Text("Objects Inside Octree: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", obj_amount);

		ImGui::Separator();	

		if (ImGui::Button("Add Current Object"))
		{
			if (App->scene_intro->GetCurrentGO() != nullptr && App->scene_intro->GetCurrentGO()->IsStatic() == true)
			{
				App->scene_intro->GetOctree()->Insert(App->scene_intro->GetCurrentGO());
				obj_amount = App->scene_intro->GetOctree()->GetNumObjects();
			}
			else
				LOG("Select a STATIC GameObject to add."); 

		}

		ImGui::End(); 
	}

}
