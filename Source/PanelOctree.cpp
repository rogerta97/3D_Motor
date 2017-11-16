#include "PanelOctree.h"
#include "Octree.h"
#include "Application.h"

void PanelOctree::Start()
{
	oct_active = false; 

	if (App->scene_intro->octree == nullptr)
	{
		min_point[0] = min_point[1] = min_point[2] = 0.0f;
		max_point[0] = max_point[1] = max_point[2] = 0.0f;
	}
	else
	{
		OctreeNode* root_node = App->scene_intro->octree->GetRootNode();

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
			ImGui::Separator(); 
			ImGui::InputInt("Objects", &max_objects);
			ImGui::Separator(); 

			ImGui::TextWrapped("This method will create an octree NO adaptative\n(Will delete the curent octree if existing)");

			ImGui::InputFloat3("Min Point", &min_point[0], 2);
			ImGui::InputFloat3("Max Point", &max_point[0], 2);

			

			if (ImGui::Button("Create"))
			{				

				App->scene_intro->octree = new Octree(); 
				App->scene_intro->octree->SetAdaptative(false);
				AABB limits(min_point, max_point);
				App->scene_intro->octree->Create(limits, max_objects);
			}

			ImGui::Separator(); 
			ImGui::TextWrapped("Creates an octree with size of 200 and center point {0,0,0}");

			if (ImGui::Button("Create adaptative"))
			{			
				min_point = { -50, -50, -50 };
				max_point = { 50, 50, 50 };
				App->scene_intro->octree = new Octree();
				App->scene_intro->octree->SetAdaptative(true); 
				AABB limits(min_point, max_point);
				App->scene_intro->octree->Create(limits, 2);
			}		

			if (App->scene_intro->octree != nullptr)
			{
				if (ImGui::Button("Delete Octree"))
				{
					App->scene_intro->octree->ClearOctree(); 
					App->scene_intro->octree = nullptr;
					delete (App->scene_intro->octree); 
					LOG("Octree Deleted Succesfully")
				}
			}

			ImGui::TreePop(); 
		}

		ImGui::Separator(); 

		if (App->scene_intro->octree != nullptr)
		{
			ImGui::Text("Min Point: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "(%.2f, %.2f, %.2f)", App->scene_intro->octree->GetRootNode()->box.minPoint.x, App->scene_intro->octree->GetRootNode()->box.minPoint.y, App->scene_intro->octree->GetRootNode()->box.minPoint.z);

			ImGui::Text("Max Point: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "(%.2f, %.2f, %.2f)", App->scene_intro->octree->GetRootNode()->box.maxPoint.x, App->scene_intro->octree->GetRootNode()->box.maxPoint.y, App->scene_intro->octree->GetRootNode()->box.maxPoint.z);
		
			App->scene_intro->octree->SetActive(oct_active);

			ImGui::Text("Objects Inside Octree: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", obj_amount);

			ImGui::Text("Max Objects per Node: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", max_objects);
		}
		else
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "No instance of Octree existing");

		ImGui::Separator();	

		if (ImGui::Button("Add Current Object"))
		{
			if (App->scene_intro->GetCurrentGO() != nullptr && App->scene_intro->GetCurrentGO()->IsStatic() == true)
			{
				bool element_inserted = false; 
				element_inserted = App->scene_intro->octree->Insert(App->scene_intro->GetCurrentGO()); 

				

				obj_amount = App->scene_intro->octree->GetNumObjects();
			}
			else
				LOG("Select a STATIC GameObject to add."); 

		}

		ImGui::End(); 
	}

}
