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

		char* name; 
		name = (char*)go_to_display->name.c_str(); 

		ImGui::InputText("Name", name, 30);

		ImGui::Separator(); 

		bool static_bool = go_to_display->IsStatic(); 
		bool active_bool = go_to_display->IsActive();

		ImGui::Checkbox("Active", &active_bool); ImGui::SameLine(); 

		ImGui::Checkbox("Static", &static_bool); ImGui::SameLine();

		if (ImGui::Button("Assign parent"))
		{
			
		}

		ImGui::Separator();

		go_to_display->SetActive(active_bool);
		go_to_display->SetStatic(static_bool);

		for (int i = 0; i < go_to_display->GetNumComponents(); i++)
		{
			switch (go_to_display->GetComponent(i)->type)
			{
			case COMPONENT_TRANSFORM:		
				PrintTransformComponent(go_to_display);
				break;
			
			case COMPONENT_MESH_RENDERER:
				PrintMeshComponent(go_to_display);
				break;

			case COMPONENT_MATERIAL:
				PrintMaterialComponent(go_to_display);
				break; 
			

			case COMPONENT_CAMERA:
				PrintCameraComponent(go_to_display);
				break; 
			

			}
			//if(ImGui::Button("Delete Component"))
		}	

		ImGui::Separator();
		ImGui::Separator();

		if (ImGui::Button("Add Component"))
		{
			show_component_popup = !show_component_popup; 			
		}

		if (show_component_popup)
		{
			int selected_fish = -1;
			const char* components[] = { "Mesh Renderer", "Material", "Camera"};

			// Simple selection popup
			// (If you want to show the current selection inside the Button itself, you may want to build a string using the "###" operator to preserve a constant ID with a variable label)		
			ImGui::OpenPopup("compos");

			if (ImGui::BeginPopup("compos"))
			{

				for (int i = 0; i < IM_ARRAYSIZE(components); i++)
					if (ImGui::Selectable(components[i]))
					{
						selected_fish = i;
						show_component_popup = false;
					}
						

				ImGui::EndPopup();
			}

			GameObject* curr_go = App->scene_intro->GetCurrentGO();

			bool replace = false;

			switch (selected_fish)
				{
					case 1: 
					{
					
						if (curr_go->GetComponent(COMPONENT_MATERIAL) != nullptr)
						{
							replace = ShowWarningModal();
						}

						if (replace || curr_go->GetComponent(COMPONENT_MATERIAL) == nullptr)
						{
							ComponentMaterial* new_mat = new ComponentMaterial(curr_go);
							curr_go->PushComponent(new_mat);
						}
					
						break;
					}
						
					case 2: 
					{
						ComponentCamera* new_cam = new ComponentCamera(curr_go, App->camera->far_plane, App->camera->near_plane, App->camera->field_of_view, App->camera->aspect_ratio);
						curr_go->PushComponent(new_cam);
						break;
					}		
				}
		}
	}
	ImGui::End();

	return true; 
}

bool PanelInspector::ShowWarningModal()
{

	bool ret = false; 

	if (ImGui::BeginPopupModal("WARNING", &modal_window, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("The component you are trying to add already exist in this object.\nDo you want to override it?");

		if (ImGui::Button("OMG DO IT"))
		{
			ret = true; 
		}

		if (ImGui::Button("FUCK IT"))
		{
			ret = false;
		}


		ImGui::EndPopup();
	}

	return ret; 
}

void PanelInspector::PrintMeshComponent(GameObject* GO_to_draw)
{
	ComponentMeshRenderer* curr_cmp = (ComponentMeshRenderer*)GO_to_draw->GetComponent(COMPONENT_MESH_RENDERER);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Mesh Renderer"))
	{
		ImGui::Text("Vertices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumVertices());
		ImGui::Text("Indices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumIndices());
		ImGui::Text("Triangles: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumTriangles());

		ImGui::Checkbox("AABB active", &curr_cmp->show_bb);

		if (curr_cmp->IsBBoxShowing())
		{
			DebugDraw(curr_cmp->GetBBox(), Red);
		}

	}
}

void PanelInspector::PrintMaterialComponent(GameObject* GO_to_draw)
{
	ComponentMaterial* curr_cmp = (ComponentMaterial*)GO_to_draw->GetComponent(COMPONENT_MATERIAL);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Material"))
	{
		ImGui::Text("Material ID: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->textures_id);

		ImTextureID img = (void*)curr_cmp->textures_id;
		ImGui::Image(img, ImVec2(100, 100));

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->width);

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->height);

		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", curr_cmp->path.c_str()); ImGui::SameLine();

	/*	if (ImGui::Button("Assign Color"))
		{
			if (ImGui::Begin("Color Editor"))
			{

				static float* st_color;
				ImGui::ColorPicker4("Ambient color##4", st_color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview, NULL);

				curr_cmp->GetColor()->Set(st_color[0], st_color[1], st_color[2], st_color[3]);

				ImGui::End(); 
			}
		}*/
	}
}

void PanelInspector::PrintTransformComponent(GameObject* GO_to_draw)
{
	ComponentTransform* curr_cmp = (ComponentTransform*)GO_to_draw->GetComponent(COMPONENT_TRANSFORM);
	ComponentCamera* cam_cmp = (ComponentCamera*)GO_to_draw->GetComponent(COMPONENT_CAMERA);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Transform"), ImGuiTreeNodeFlags_Leaf)
	{

		if (ImGui::Button("Reset Transform"))
		{
			curr_cmp->SetLocalPosition(float3(0, 0, 0));
			curr_cmp->SetLocalRotation(DegToRad(float3(0, 0, 0)));
			curr_cmp->SetLocalScale(float3(1, 1, 1));

			if (curr_cmp->GetComponentParent()->GetNumChilds() > 0)
			{
				GameObject* curr_go = curr_cmp->GetComponentParent();

				for (int i = 0; i < curr_go->GetNumChilds(); i++)
				{
					ComponentTransform* trans = (ComponentTransform*)curr_go->GetChild(i)->GetComponent(COMPONENT_TRANSFORM);

					trans->SetLocalPosition(float3(0, 0, 0));
					trans->SetLocalRotation(DegToRad(float3(0, 0, 0)));
					trans->SetLocalScale(float3(1, 1, 1));

				}
			}
			return;
		}

		static int e = 0;
		ImGui::RadioButton("Local", &e, 0); 

		if (curr_cmp->GetComponentParent()->GetParent() != nullptr)
		{
			ImGui::SameLine();
			ImGui::RadioButton("Global", &e, 1);
		}

		float3 radians_angle;
		static float pos[3];
		static float rot[3];
		static float s[3];

		switch (e)
		{
		case 0:
			radians_angle = curr_cmp->GetLocalRotation().ToEulerXYZ();

			pos[0] = curr_cmp->GetLocalPosition().x; pos[1] = curr_cmp->GetLocalPosition().y; pos[2] = curr_cmp->GetLocalPosition().z;
			rot[0] = RadToDeg(radians_angle.x); rot[1] = RadToDeg(radians_angle.y); rot[2] = RadToDeg(radians_angle.z);
			s[0] = curr_cmp->GetLocalScale().x; s[1] = curr_cmp->GetLocalScale().y; s[2] = curr_cmp->GetLocalScale().z;
			if (ImGui::DragFloat3("Position##transform", pos, 2))
			{
				curr_cmp->SetLocalPosition(float3(pos[0], pos[1], pos[2]));
				//cam_cmp->frustum.SetPos(vec(pos[0], pos[1], pos[2]));
			}

			if (ImGui::SliderFloat3("Rotation##transform", rot, 0.0f, 180.0f))
			{
				curr_cmp->SetLocalRotation(DegToRad(float3(rot[0], rot[1], rot[2])));
			}
				

			if (ImGui::DragFloat3("Scale##transform", s, 2))
				curr_cmp->SetLocalScale(float3(s[0], s[1], s[2]));


			break;

		case 1:

			radians_angle = curr_cmp->GetGlobalRotation().ToEulerXYZ();

			pos[0] = curr_cmp->GetGlobalPosition().x; pos[1] = curr_cmp->GetGlobalPosition().y; pos[2] = curr_cmp->GetGlobalPosition().z;
			rot[0] = RadToDeg(radians_angle.x); rot[1] = RadToDeg(radians_angle.y); rot[2] = RadToDeg(radians_angle.z);
			s[0] = curr_cmp->GetGlobalScale().x; s[1] = curr_cmp->GetGlobalScale().y; s[2] = curr_cmp->GetGlobalScale().z;
			
			ImGui::Text("----- Read Only -----"); 

			ImGui::DragFloat3("Position##transform", pos, 2);
			ImGui::DragFloat3("Rotation##transform", rot, 0.0f, 180.0f);
			ImGui::DragFloat3("Scale##transform", s, 2);
		
			break;
		}

	


	
	}
}

void PanelInspector::PrintCameraComponent(GameObject* GO_to_draw)
{
	ComponentCamera* curr_cmp = (ComponentCamera*)GO_to_draw->GetComponent(COMPONENT_CAMERA);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Camera"))
	{
		ImGui::DragFloat("Near Plane Distance##transform", &curr_cmp->frustum.nearPlaneDistance, 0.1f, 0.0f);
		//curr_cmp->SetNearPlaneDist(nearPlane_aux);

		ImGui::DragFloat("Far Plane Distance##transform", &curr_cmp->frustum.farPlaneDistance, 0.1f, 0.0f);
		//curr_cmp->SetNearPlaneDist(nearPlane_aux);
	}
}
		