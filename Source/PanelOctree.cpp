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
		ImGui::Checkbox("Active", &oct_active); 

		App->scene_intro->GetOctree()->SetActive(oct_active); 

		ImGui::Separator(); 

		ImGui::InputFloat3("Min Point", &min_point[0], 2);
		ImGui::InputFloat3("Max Point", &max_point[0], 2);

		ImGui::Text("Objects Inside Octree: "); ImGui::SameLine(); 
		
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", obj_amount);

		if(ImGui::Button("Create!"))
		{
		/*	if (App->scene_intro->GetOctree() != nullptr)
			{
				App->scene_intro->GetOctree()->GetRootNode()->DeleteNode();
			}
		
			App->scene_intro->GetOctree()->Create(AABB(vec(min_point[0], min_point[1], min_point[2]), vec(max_point[0], max_point[1], max_point[2])));*/
			
		}

		if (ImGui::Button("Add Current Object"))
		{
			App->scene_intro->GetOctree()->Insert(App->scene_intro->GetCurrentGO()); 		
			App->scene_intro->GetOctree()->GetRootNode()->GetObjectsInNode(obj_amount);
		}

		ImGui::End(); 
	}

}
