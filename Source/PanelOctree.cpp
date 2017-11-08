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

			if (ImGui::Button("Create"))
			{				
				App->scene_intro->octree = new Octree(); 
				AABB limits(min_point, max_point);
				App->scene_intro->octree->Create(limits);
			}

			ImGui::Separator(); 
			ImGui::TextWrapped("Creates an octree with size of 200 and center point {0,0,0}");

			if (ImGui::Button("Create default"))
			{
				min_point = { -100, -100, -100 }; 
				max_point = { 100, 100, 100 }; 
				App->scene_intro->octree = new Octree();
				AABB limits(min_point, max_point);
				App->scene_intro->octree->Create(limits);
			}

		
			

			ImGui::TreePop(); 
		}

		if (App->scene_intro->octree != nullptr)
		{
			ImGui::Text("Min Point: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "(%.2f, %.2f, %.2f)", App->scene_intro->octree->GetRootNode()->box.minPoint.x, App->scene_intro->octree->GetRootNode()->box.minPoint.y, App->scene_intro->octree->GetRootNode()->box.minPoint.z);

			ImGui::Text("Max Point: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "(%.2f, %.2f, %.2f)", App->scene_intro->octree->GetRootNode()->box.maxPoint.x, App->scene_intro->octree->GetRootNode()->box.maxPoint.y, App->scene_intro->octree->GetRootNode()->box.maxPoint.z);
		}

		if(App->scene_intro->octree!= nullptr)
			App->scene_intro->octree->SetActive(oct_active); 
 
		ImGui::Text("Objects Inside Octree: "); ImGui::SameLine(); 
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", obj_amount);

		ImGui::Separator();	

		if (ImGui::Button("Add Current Object"))
		{
			if (App->scene_intro->GetCurrentGO() != nullptr && App->scene_intro->GetCurrentGO()->IsStatic() == true)
			{
				App->scene_intro->octree->Insert(App->scene_intro->GetCurrentGO());
				obj_amount = App->scene_intro->octree->GetNumObjects();
			}
			else
				LOG("Select a STATIC GameObject to add."); 

		}

		ImGui::End(); 
	}

}
