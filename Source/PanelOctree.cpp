#include "PanelOctree.h"

void PanelOctree::Start()
{
	oct_active = false; 

	min_point[0] = min_point[1] = min_point[2] = 0.0f; 
	max_point[0] = max_point[1] = max_point[2] = 0.0f;

}

void PanelOctree::DrawPanel()
{
	if (ImGui::Begin("Octree"))
	{
		ImGui::Checkbox("Active", &oct_active); 

		ImGui::Separator(); 

		ImGui::InputFloat3("Min Point", &min_point[0], 2);
		ImGui::InputFloat3("Max Point", &max_point[0], 2);

		if(ImGui::Button("Create!"))
		{

		}

		ImGui::End(); 
	}

}
