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

	if (ImGui::Begin("Inspector"), &show_inspector && !App->scene_intro->IsSceneEmpty() && App->scene_intro->GetCurrentGO() != nullptr)
	{
		GameObject* go_to_display = App->scene_intro->GetCurrentGO();

		char* name; 
		name = (char*)go_to_display->name.c_str(); 

		ImGui::InputText("Name", name, 30);

		ImGui::Separator(); 

		bool static_bool = go_to_display->IsStatic(); 
		bool active_bool = go_to_display->IsActive();

		ImGui::Checkbox("Active", &active_bool); ImGui::SameLine(); 

		bool keeper = static_bool; 

		ImGui::Checkbox("Static", &static_bool); ImGui::SameLine();

		if (keeper != static_bool)
		{
			if (static_bool) go_to_display->SetStatic(true);
			else go_to_display->SetStatic(false);
		}

		if (go_to_display->parent == nullptr)
		{
			if (ImGui::Button("Assign parent"))
			{
				App->scene_intro->GetCurrentGO()->SetLooking4Parent(true);
			}
		}
		else
		{
			if (ImGui::Button("Delete parent"))
			{
				go_to_display->DeleteParent(); 
			}
		}
	
		

		ImGui::Separator();

		go_to_display->SetActive(active_bool);
		//go_to_display->is_static = static_bool;

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

			case COMPONENT_BILLBOARDING:
				PrintBillBoardingComponent(go_to_display);
				break;

			case COMPONENT_PARTICLE_EMMITER:
				PrintComponentParticleEmmiter(go_to_display);
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
			const char* components[] = { "Mesh Renderer", "Material", "Camera", "Billboarding", "Particle Emmiter"};

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
						if (replace || curr_go->GetComponent(COMPONENT_MATERIAL) == nullptr)
						{
							ComponentMaterial* new_mat = new ComponentMaterial(curr_go);
							curr_go->PushComponent(new_mat);
						}
					
						break;
					}
						
					case 2: 
					{
						ComponentCamera* new_cam = new ComponentCamera(curr_go, 30.0f, 0.1f, 60.0f, 1.5f);
						curr_go->PushComponent(new_cam);
						break;
					}	

					case 3:
					{
						ComponentBillboarding* new_bill = new ComponentBillboarding(curr_go);
						curr_go->PushComponent(new_bill);
						break;
					}

					case 4:
					{
						ComponentParticleEmmiter* new_emmiter = new ComponentParticleEmmiter(curr_go);
						curr_go->PushComponent(new_emmiter);
						break;
					}
				}
		}
	}
	ImGui::End();

	return true; 
}


void PanelInspector::PrintMeshComponent(GameObject* GO_to_draw)
{
	ComponentMeshRenderer* curr_cmp = (ComponentMeshRenderer*)GO_to_draw->GetComponent(COMPONENT_MESH_RENDERER);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Mesh Renderer"))
	{

		if (ImGui::Button("Delete Component"))
		{
			GO_to_draw->DeleteComponent(COMPONENT_MESH_RENDERER);
		}

		ImGui::Text("Vertices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumVertices());
		ImGui::Text("Indices: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumIndices());
		ImGui::Text("Triangles: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumTriangles());

		ImGui::Text("Normals: "); ImGui::SameLine();
		if(curr_cmp->normals_id != 0)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->GetNumNormals());
		else
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "NONE");

		ImGui::Separator();

		ImGui::Checkbox("Draw AABB", &curr_cmp->show_bb); ImGui::SameLine(); 

		if(curr_cmp->normals_id != 0)
			ImGui::Checkbox("Draw Normals", &curr_cmp->show_normals);

	}

	if (curr_cmp->IsBBoxShowing())
		DebugDraw(curr_cmp->GetBBox(), Red);
	
	if (curr_cmp->AreNormalsShowing())
	{
		App->renderer3D->DrawNormals(curr_cmp->normals, curr_cmp->center_points, curr_cmp->num_normals);
	}
}

void PanelInspector::PrintMaterialComponent(GameObject* GO_to_draw)
{
	ComponentMaterial* curr_cmp = (ComponentMaterial*)GO_to_draw->GetComponent(COMPONENT_MATERIAL);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Material"))
	{

		if (ImGui::Button("Delete Component"))
		{
			GO_to_draw->DeleteComponent(COMPONENT_MATERIAL);
		}

		ImGui::Text("Material ID: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->textures_id);

		ImTextureID img = (void*)curr_cmp->textures_id;
		ImGui::Image(img, ImVec2(100, 100),ImVec2(1,1), ImVec2(0,0));

		ImGui::Text("Width: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->width);

		ImGui::Text("Height: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", curr_cmp->height);

		if (ImGui::Button("Select New Resource"))
			show_resources_popup = true;
	
		ImGui::Text("Name: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", curr_cmp->path.c_str()); ImGui::SameLine();

		if (show_resources_popup)
			ShowMaterialResources(); 
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

		float3 radians_angle;
		static float pos[3];
		static float rot[3];
		static float s[3];

		radians_angle = curr_cmp->GetLocalRotation().ToEulerXYZ();

		pos[0] = curr_cmp->GetLocalPosition().x; pos[1] = curr_cmp->GetLocalPosition().y; pos[2] = curr_cmp->GetLocalPosition().z;
		rot[0] = RadToDeg(radians_angle.x); rot[1] = RadToDeg(radians_angle.y); rot[2] = RadToDeg(radians_angle.z);
		s[0] = curr_cmp->GetLocalScale().x; s[1] = curr_cmp->GetLocalScale().y; s[2] = curr_cmp->GetLocalScale().z;

		float new_pos[3] = { pos[0], pos[1], pos[2] };
		float new_rot[3] = { rot[0], rot[1], rot[2] };
		float new_scale[3] = { s[0], s[1], s[2] };

		if(ImGui::DragFloat3("Position##transform", new_pos, 0.2f))
			curr_cmp->SetLocalPosition(float3(new_pos[0], new_pos[1], new_pos[2]));

		if(ImGui::DragFloat3("Rotation##transform", new_rot, 0.2f))
			curr_cmp->SetLocalRotation(DegToRad(float3(new_rot[0], new_rot[1], new_rot[2])));

		if (ImGui::DragFloat3("Scale##transform", new_scale, 0.2f))
		{
			if (new_scale[0] < 0.2f)
				new_scale[0] = 0.2f;
			if (new_scale[1] < 0.2f)
				new_scale[1] = 0.2f;
			if (new_scale[2] < 0.2f)
				new_scale[2] = 0.2f;			
		}	

		if (new_pos[0] != pos[0] || new_pos[1] != pos[1] || new_pos[2] != pos[2])
		{
			curr_cmp->SetModified(true);
		}
			

		// HAVE TO ASK WHY ROTATION IS NOT EXACTLY 10 IS 9.9999
		if (new_rot[0] != rot[0] || new_rot[1] != rot[1] || new_rot[2] != rot[2])
		{
			curr_cmp->SetModified(true);
		}
		
			
		if (new_scale[0] != s[0] || new_scale[1] != s[1] || new_scale[2] != s[2])
			curr_cmp->SetLocalScale(float3(new_scale[0], new_scale[1], new_scale[2]));
	}
}

void PanelInspector::PrintCameraComponent(GameObject* GO_to_draw)
{
	ComponentCamera* curr_cmp = (ComponentCamera*)GO_to_draw->GetComponent(COMPONENT_CAMERA);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Camera"))
	{

		if (ImGui::Button("Delete Component"))
		{
			GO_to_draw->DeleteComponent(COMPONENT_CAMERA);
		}

		ImGui::Checkbox("Frustum Culling", &curr_cmp->frustum_culling);

		if(ImGui::DragFloat("Near Plane Distance##transform", &curr_cmp->frustum.nearPlaneDistance, 0.1f, 0.0f))
			curr_cmp->frustum.SetViewPlaneDistances(curr_cmp->frustum.nearPlaneDistance, curr_cmp->frustum.farPlaneDistance);

		if(ImGui::DragFloat("Far Plane Distance##transform", &curr_cmp->frustum.farPlaneDistance, 0.1f, 0.0f))
			curr_cmp->frustum.SetViewPlaneDistances(curr_cmp->frustum.nearPlaneDistance, curr_cmp->frustum.farPlaneDistance);
		
		if (ImGui::SliderFloat("Field Of View##transform", &curr_cmp->frustum.verticalFov, 0.1f, 179.0f,"%.2f"))
			curr_cmp->frustum.SetVerticalFovAndAspectRatio(curr_cmp->frustum.verticalFov, curr_cmp->frustum.AspectRatio());
		
	}
}

void PanelInspector::PrintBillBoardingComponent(GameObject * Go_to_draw)
{
	ComponentBillboarding* new_bill = (ComponentBillboarding*)Go_to_draw->GetComponent(COMPONENT_BILLBOARDING); 

	if (ImGui::CollapsingHeader("Component Billboarding"))
	{
		if (ImGui::Button("Set Reference"))
		{	
			new_bill->SetShowInputWindow(true);
		}

		if (new_bill->GetShowInputWindow())
		{
			App->camera->LockCamera(); 

			static char ref_object_name[30]; 

			ImGui::Begin("Attach Reference"); 

			ImGui::InputText("Referece", &ref_object_name[0], 30); 

			if (ImGui::Button("Set"))
			{
				if (strlen(ref_object_name) != 0)
				{
					GameObject* new_reference = App->scene_intro->FindByNameRecursive(ref_object_name);
					new_bill->SetReference(new_reference);

					new_reference->transform->SetPositionChanged(true); 
				}

				new_bill->SetShowInputWindow(false);
			}

			ImGui::End(); 
		}

		ImGui::Text("Attached to:"); ImGui::SameLine();

		if (new_bill->GetReference() != nullptr)
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", new_bill->GetReference()->GetName());
		else
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "NONE"); 

		ImGui::Separator();

		if (new_bill->GetReference() != nullptr)
		{
			ImGui::Checkbox("Lock Y", &new_bill->y_axis_locked);
			ImGui::Checkbox("Lock X", &new_bill->x_axis_locked);
		}
	}	
}

void PanelInspector::PrintComponentParticleEmmiter(GameObject * Go_to_draw)
{
	ComponentParticleEmmiter* new_emm = (ComponentParticleEmmiter*)Go_to_draw->GetComponent(COMPONENT_PARTICLE_EMMITER);

	if (ImGui::CollapsingHeader("Component Particle Emmiter"))
	{
		bool active_bool = new_emm->IsActive();
		bool keeper = active_bool;

		ImGui::Checkbox("Active", &active_bool);

		if (keeper != active_bool)
			new_emm->SetActive(keeper);

		if (new_emm->IsActive())
		{
			ImGui::Separator();

			if (ImGui::Button("PLAY"))
			{

			}
			ImGui::SameLine(); 

			if (ImGui::Button("STOP"))
			{

			}

			ImGui::Separator();

			static int particle_template; 
			ImGui::Combo("Templates", &particle_template, "Select Template\0Smoke\0Custom\0");
		
			switch (particle_template)
			{
			case 1: 
				//Here we set properties for the particles to look like smoke 

				break; 
			}

			ImGui::Separator();

			if (ImGui::TreeNode("Shape"))
			{
				ImGui::TreePop(); 
			}

			if (ImGui::TreeNode("Color"))
			{
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Motion"))
			{
	
				static bool show = new_emm->ShowEmmisionArea();
				ImGui::Checkbox("Show Emmiter Area", &show);
				new_emm->SetShowEmmisionArea(show);

				int emision_rate = new_emm->GetEmmisionRate(); 
				ImGui::DragInt("Emmision Rate", &emision_rate, 1, 0, 150);
				new_emm->SetEmmisionRate(emision_rate);

				float lifetime = new_emm->GetLifetime();
				ImGui::DragFloat("Lifetime", &lifetime, 0, 0, 20);
				new_emm->SetLifeTime(lifetime);

				ImGui::TreePop();
			}

			ImGui::Separator();

			if (ImGui::Button("Save as Template"))
			{

			}

	
		
		}
	}

}

void PanelInspector::ShowMaterialResources()
{
	ImGui::Begin("Resources");
	
	vector<string> files; 
	App->file_system->GetAllFilesInDirectory(App->file_system->tex_path_game.c_str(), files);

	for (int i = 0; i < files.size(); i++)
	{
		if (ImGui::MenuItem(files[i].c_str()))
		{
			string path(App->file_system->tex_path_game + files[i]);
			App->resource_manager->Load(path.c_str());
			show_resources_popup = false; 
		}
	}
		
	ImGui::End();
}
		