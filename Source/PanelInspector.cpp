#include "PanelInspector.h"

#include "GameObject.h"
#include "Application.h"
#include "ComponentDefs.h"

bool PanelInspector::Start()
{
	return false;
}

bool PanelInspector::Update()
{
	return false;
}

bool PanelInspector::Draw()
{
	if (!show_inspector)
		return false;

	if (ImGui::Begin("Inspector"), &show_inspector && App->scene_intro->GetGameObject(0) != nullptr)
	{
		GameObject* go_to_display = App->scene_intro->GetCurrentGO();

		ImGui::Text("%s", go_to_display->GetName()); 

		for (int i = 0; i < go_to_display->GetNumComponents(); i++)
		{
			switch (go_to_display->GetComponent(i)->type)
			{
			case COMPONENT_TRANSFORM:
			{		
					ComponentTransform* curr_cmp = (ComponentTransform*)go_to_display->GetComponent(COMPONENT_TRANSFORM);

					if (curr_cmp == nullptr)
						continue;

					if (ImGui::CollapsingHeader("Component Transform"))
					{

						if (ImGui::Button("Reset Transform"))
						{
							curr_cmp->SetPosition(float3(0,0,0));
							curr_cmp->SetRotation(DegToRad(float3(0,0,0)));
							curr_cmp->SetScale(float3(1,1,1));
							continue; 
						}

					float3 radians_angle = curr_cmp->GetLocalRotation().ToEulerXYZ();

					float pos[3] = { curr_cmp->GetLocalPosition().x,curr_cmp->GetLocalPosition().y,curr_cmp->GetLocalPosition().z };
					float rot[3] = { RadToDeg(radians_angle.x),RadToDeg(radians_angle.y),RadToDeg(radians_angle.z), };
					float s[3] = { curr_cmp->GetLocalScale().x,curr_cmp->GetLocalScale().y,curr_cmp->GetLocalScale().z };

					if (ImGui::InputFloat3("Position##transform", pos, 2))
						curr_cmp->SetPosition(float3(pos[0], pos[1], pos[2]));
					if (ImGui::InputFloat3("Rotation##transform", rot, 2))
						curr_cmp->SetRotation(DegToRad(float3(rot[0], rot[1], rot[2])));
					if (ImGui::InputFloat3("Scale##transform", s, 2))
						curr_cmp->SetScale(float3(s[0], s[1], s[2]));
					}

				break;
			}

			case COMPONENT_MESH_RENDERER:
			{
				ComponentMeshRenderer* curr_cmp = (ComponentMeshRenderer*)go_to_display->GetComponent(COMPONENT_MESH_RENDERER);

				if (curr_cmp == nullptr)
					continue;
				if (ImGui::CollapsingHeader("Component Mesh Renderer"))
				{
					ImGui::Text("Vertices: "); ImGui::SameLine(); 
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->num_vertices);
					ImGui::Text("Indices: "); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->num_indices);
				}
		
			}

				break;

			case COMPONENT_MATERIAL:
			{
				ComponentMaterial* curr_cmp = (ComponentMaterial*)go_to_display->GetComponent(COMPONENT_MATERIAL);

				if (curr_cmp == nullptr)
					continue;

				if (ImGui::CollapsingHeader("Component Material"))
				{
					ImGui::Text("Material ID: "); ImGui::SameLine(); 
					ImGui::TextColored(ImVec4(1,1,0,1), "%d", curr_cmp->textures_id);

					ImGui::Text("Width: "); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->width);

					ImGui::Text("Height: "); ImGui::SameLine();
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->height);

					ImGui::Text("Name: "); ImGui::SameLine(); 
					ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s");

				}
	
				break;
			}

			case COMPONENT_CAMERA:
			{
				ComponentCamera* curr_cmp = (ComponentCamera*)go_to_display->GetComponent(COMPONENT_CAMERA);

				if (curr_cmp == nullptr)
					continue;

				if (ImGui::CollapsingHeader("Component Camera"))
				{

				}

				break;
			}

			}


		}	
	}
	ImGui::End();

	return true; 
}
		

//	
//			for (int i = 0; i < go_to_display->GetNumMeshes(); i++)
//			{
//				string secction;
//
//				if (go_to_display->GetNumMeshes() == 1)
//					secction = go_to_display->GetName();
//				else
//				{
//					secction = "Section: ";
//					secction += to_string(i + 1);
//				}
//
//				if (ImGui::CollapsingHeader(secction.c_str()))
//				{
//					ComponentTransform* ct_aux = (ComponentTransform*)go_to_display->GetComponent(COMPONENT_TRANSFORM, i);
//
//					if (ct_aux != nullptr)
//					{
//						string title("Component Transform ");
//						title += '('; title += to_string(i + 1); title += ')';
//
//						if (ImGui::TreeNode(title.c_str()))
//						{
//							float3 radians_angle = ct_aux->GetLocalRotation().ToEulerXYZ();
//
//							float pos[3] = { ct_aux->GetLocalPosition().x,ct_aux->GetLocalPosition().y,ct_aux->GetLocalPosition().z };
//							float rot[3] = { RadToDeg(radians_angle.x),RadToDeg(radians_angle.y),RadToDeg(radians_angle.z), };
//							float s[3] = { ct_aux->GetLocalScale().x,ct_aux->GetLocalScale().y,ct_aux->GetLocalScale().z };
//
//							if (ImGui::InputFloat3("Pos##transform", pos, 2))
//								ct_aux->SetPosition(float3(pos[0], pos[1], pos[2]));
//							if (ImGui::InputFloat3("Rot##transform", rot, 2))
//								ct_aux->SetRotation(DegToRad(float3(rot[0], rot[1], rot[2])));
//							if (ImGui::InputFloat3("Scale##transform", s, 2))
//								ct_aux->SetScale(float3(s[0], s[1], s[2]));
//
//
//							ImGui::TreePop();
//						}
//						ImGui::Separator();
//					}
//
//
//
//					ComponentMeshRenderer* cmr_aux = (ComponentMeshRenderer*)go_to_display->GetComponent(COMPONENT_MESH_RENDERER, i);
//
//					if (cmr_aux != nullptr)
//					{
//						string title("Component Mesh Renderer ");
//						title += '('; title += to_string(i + 1); title += ')';
//
//						if (ImGui::TreeNode(title.c_str()))
//						{
//
//							ImGui::Text("Vertices: "); ImGui::SameLine();
//							ImGui::TextColored(ImVec4{ 1,1,0,1 }, "%d", cmr_aux->num_vertices);
//							ImGui::Text("Indices: "); ImGui::SameLine();
//							ImGui::TextColored(ImVec4{ 1,1,0,1 }, "%d", cmr_aux->num_indices);
//
//							ImGui::TreePop();
//						}
//						ImGui::Separator();
//					}
//
//					ComponentMaterial* cm_aux = (ComponentMaterial*)go_to_display->GetComponent(COMPONENT_MATERIAL, i);
//
//					if (cm_aux != nullptr)
//					{
//						string title("Component Material ");
//						title += '('; title += to_string(i + 1); title += ')';
//
//						if (ImGui::TreeNode(title.c_str()))
//						{
//
//							ImGui::Text("Texture ID: "); ImGui::SameLine();
//							ImGui::TextColored(ImVec4{ 1,1,0,1 }, "%d", cm_aux->textures_id);
//							ImGui::Text("Texture Width: "); ImGui::SameLine();
//							ImGui::TextColored(ImVec4{ 1,1,0,1 }, "%d", cm_aux->width);
//							ImGui::Text("Texture Height: "); ImGui::SameLine();
//							ImGui::TextColored(ImVec4{ 1,1,0,1 }, "%d", cm_aux->height);
//							ImGui::Text("Path: ");
//							ImGui::TextColored(ImVec4{ 1,1,0,1 }, "%s", cm_aux->path);
//
//							ImGui::TreePop();
//						}
//						ImGui::Separator();
//					}
//				
//			}
//		}
//
//		else
//		{
//
//		}
//		ImGui::End();
//	}
//
//	return true; 
//}

