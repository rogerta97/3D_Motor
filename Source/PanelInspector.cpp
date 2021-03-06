#include "PanelInspector.h"
#include "GameObject.h"
#include "Application.h"
#include "ComponentDefs.h"
#include <math.h>

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

		ImGui::Separator();

		ImGui::Checkbox("Draw AABB", &curr_cmp->show_bb); ImGui::SameLine(); 


	}

	if (curr_cmp->IsBBoxShowing())
		DebugDraw(curr_cmp->GetBBox(), Red);
	
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

		rot[0] = radians_angle.x*RADTODEG; 
		rot[1] = radians_angle.y*RADTODEG;
		rot[2] = radians_angle.z*RADTODEG;

		if(ImGui::DragFloat3("Position##transform", (float*)pos, 0.2f))
			curr_cmp->SetLocalPosition(float3(pos[0], pos[1], pos[2]));

		ImGui::SliderFloat3("Rotation##transform", (float*)rot, 0, 360.0f); 
	
		if (ImGui::DragFloat3("Scale##transform", (float*)s, 0.2f))
		{
			if (s[0] < 0.2f)
				s[0] = 0.2f;
			if (s[1] < 0.2f)
				s[1] = 0.2f;
			if (s[2] < 0.2f)
				s[2] = 0.2f;

			curr_cmp->SetLocalScale({s[0], s[1], s[2]});
		}	

		Quat new_rot = Quat::FromEulerXYZ(rot[0], rot[1], rot[2]); 

		if (rot[0] != radians_angle.x*RADTODEG)
			LOG("0"); 

		if (rot[1] != radians_angle.y*RADTODEG)
			LOG("1");

		if (rot[2] != radians_angle.z*RADTODEG)
			LOG("2");

		if (curr_cmp->transform.rotation.Equals(new_rot) == false)
		{
			float3 increment = { rot[0] - radians_angle.x*RADTODEG, rot[1] - radians_angle.y*RADTODEG , rot[2] - radians_angle.z*RADTODEG };
			curr_cmp->SetLocalRotation(increment);
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
	ComponentBillboarding* curr_cmp = (ComponentBillboarding*)Go_to_draw->GetComponent(COMPONENT_BILLBOARDING);

	if (curr_cmp == nullptr)
		return;

	if (ImGui::CollapsingHeader("Component Billboarding"))
	{

		bool prev_check = curr_cmp->bill_active; 
		ImGui::Checkbox("Billboarding Active", &curr_cmp->bill_active);

		if (prev_check != curr_cmp->bill_active)
		{
			if(curr_cmp->bill_active == true)
			{
				curr_cmp->SetReference(App->renderer3D->rendering_cam);
				//curr_cmp->ref_position_changed = true;
			}
			else
			{
				curr_cmp->SetReference(nullptr);
				curr_cmp->GetComponentParent()->transform->SetIdentityTransform(); 
			}
		
		}

		ImGui::Separator(); 

		ImGui::Checkbox("Lock Y", &curr_cmp->y_axis_locked); ImGui::SameLine(); 
		ImGui::Checkbox("Lock X", &curr_cmp->x_axis_locked);

	}
}

void PanelInspector::PrintComponentParticleEmmiter(GameObject * Go_to_draw)
{
	ComponentParticleEmmiter* current_emmiter = (ComponentParticleEmmiter*)Go_to_draw->GetComponent(COMPONENT_PARTICLE_EMMITER);

	if (ImGui::CollapsingHeader("Component Particle Emmiter"))
	{
		bool active_bool = current_emmiter->IsActive();
		bool keeper = active_bool;

		ImGui::Checkbox("Active", &active_bool);

		if (keeper != active_bool)
			current_emmiter->SetActive(keeper);

		if (current_emmiter->IsActive())
		{
			ImGui::Separator();

			static int particle_template;
			ImGui::Combo("Templates", &particle_template, "Select Template\0Smoke\0Custom\0");

			if (particle_template == 0)
				return;

			switch (particle_template)
			{
			case 1:
				//Here we set properties for the particles to look like smoke 

				break;
			}

			bool update_root = false;

			if (ImGui::Button("PLAY"))
			{
				current_emmiter->SetSystemState(PARTICLE_STATE_PLAY);
				current_emmiter->Start();
			}

			ImGui::SameLine();

			if (ImGui::Button("STOP"))
			{
				current_emmiter->SetSystemState(PARTICLE_STATE_PAUSE);
			}

			ImGui::Separator();

			if (ImGui::TreeNode("Emit Area"))
			{

				static bool show = current_emmiter->ShowEmmisionArea();
				ImGui::Checkbox("Show Emmiter Area", &show);
				current_emmiter->SetShowEmmisionArea(show);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Shape"))
			{

				//Here we get the id of the BUTTON that is pressed, the position. 
				uint button_pressed = -1;
				for (int i = 0; i < current_emmiter->GetTextureIDAmount(); i++)
				{
					if (ImGui::ImageButton((ImTextureID)current_emmiter->GetTextureID(i), ImVec2(32, 32), ImVec2(1, 1), ImVec2(0, 0), 2, ImColor(0, 0, 0, 255)))
						button_pressed = i;

					if (i != current_emmiter->GetTextureIDAmount() - 1)
						ImGui::SameLine();
				}

				//Once we have the position we can get the texture id associated with it 
				if (button_pressed != -1)
				{
					uint texture_to_display = current_emmiter->GetTextureID(button_pressed);
					current_emmiter->SetCurrentTextureID(texture_to_display);
					current_emmiter->UpdateRootParticle();
				}

				ImGui::Separator(); 

				ImGui::Text("Animated particles"); 

				//Load the animated particles of the engine by default
				vector<ParticleAnimation> particle_anims = current_emmiter->GetAllParticleAnimations(); 

				for (int i = 0; i < particle_anims.size(); i++)
				{
					if (ImGui::TreeNode(particle_anims[i].name.c_str()))
					{
						for (int j = 0; j < particle_anims[i].buffer_ids.size(); j++)
						{
							ImTextureID id = (ImTextureID)particle_anims[i].buffer_ids[j];

							ImGui::ImageButton(id, ImVec2(32, 32), ImVec2(1, 1), ImVec2(0, 0), 2, ImColor(0, 0, 0, 255));
							ImGui::SameLine(); 
						}
						ImGui::NewLine();

						ImGui::DragFloat("Time Step", &current_emmiter->time_step, 0.05, 0.05f, 1.0f); 

						if (ImGui::Button("Set"))
						{
							current_emmiter->is_animated = true; 
							current_emmiter->GetRootParticle()->components.particle_animation = particle_anims[i]; 
							current_emmiter->GetRootParticle()->components.particle_animation.timeStep = current_emmiter->time_step; 

						} ImGui::SameLine(); 

						if (ImGui::Button("Stop"))
						{
							current_emmiter->is_animated = false;

						}

						ImGui::TreePop(); 
					}

				
				}
				ImGui::Separator(); 
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Color"))
			{

				static bool alpha_preview = true;
				ImGui::Checkbox("Alpha", &alpha_preview);

				int misc_flags = (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0);

				ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar;
				flags |= misc_flags;
				flags |= ImGuiColorEditFlags_RGB;

				ImGui::ColorPicker4("Current Color##4", (float*)&current_emmiter->color, flags);

				if (ImGui::TreeNode("Color Interpolation"))
				{
					current_emmiter->UpdateRootParticle();

					ImGui::DragInt4("Initial Color", &current_emmiter->initial_color[0], 1, 0, 255);

					if (ImGui::Button("Set Selected as Initial"))
					{
						current_emmiter->initial_color[0] = current_emmiter->color.r * 255;
						current_emmiter->initial_color[1] = current_emmiter->color.g * 255;
						current_emmiter->initial_color[2] = current_emmiter->color.b * 255;
						current_emmiter->initial_color[3] = current_emmiter->color.a * 255;
					}

					ImGui::DragInt4("Final Color", &current_emmiter->final_color[0], 1, 0, 255);

					if (ImGui::Button("Set Selected as Final"))
					{

						current_emmiter->final_color[0] = current_emmiter->color.r * 255;
						current_emmiter->final_color[1] = current_emmiter->color.g * 255;
						current_emmiter->final_color[2] = current_emmiter->color.b * 255;
						current_emmiter->final_color[3] = current_emmiter->color.a * 255;
					}

					ImGui::Separator();

					if (ImGui::Button("Apply"))
					{
						current_emmiter->apply_color_interpolation = true;

						Color initial(current_emmiter->initial_color[0], current_emmiter->initial_color[1], current_emmiter->initial_color[2], current_emmiter->initial_color[3]);
						Color final(current_emmiter->final_color[0], current_emmiter->final_color[1], current_emmiter->final_color[2], current_emmiter->final_color[3]);

						current_emmiter->GetRootParticle()->SetInitialColor(initial);
						current_emmiter->GetRootParticle()->SetFinalColor(final);

						current_emmiter->UpdateRootParticle();
					}

					ImGui::TreePop();
				}

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Motion"))
			{
				ImGui::Checkbox("Billboarding", &current_emmiter->billboarding);

				if (ImGui::DragInt("Emmision Rate", &current_emmiter->emmision_rate, 1, 0, 150)) current_emmiter->UpdateRootParticle();
				if (ImGui::DragFloat("Lifetime", &current_emmiter->max_lifetime, 0, 0, 20)) current_emmiter->UpdateRootParticle();
				if (ImGui::SliderFloat("Velocity", &current_emmiter->velocity, 0.1f, 5)) current_emmiter->UpdateRootParticle();
				if (ImGui::SliderFloat3("Gravity", &current_emmiter->gravity[0], -5, 5)) current_emmiter->UpdateRootParticle();
				if (ImGui::DragFloat("Angular Velocity", &current_emmiter->angular_v, 1, 0, 200)) current_emmiter->UpdateRootParticle();
				ImGui::DragFloat("Emision Angle", &current_emmiter->emision_angle, 1, 0, 360); 

				ImGui::Separator();

				ImGui::Text("Size Interpolation");

				ImGui::InputFloat("Initial", &current_emmiter->initial_scale.x);
				current_emmiter->initial_scale.y = current_emmiter->initial_scale.x;

				ImGui::InputFloat("Final", &current_emmiter->final_scale.x);
				current_emmiter->final_scale.y = current_emmiter->final_scale.x;

				if (ImGui::Button("Apply Scale"))
				{
					current_emmiter->apply_size_interpolation = true; 
					current_emmiter->UpdateRootParticle();
				}
		
				ImGui::Separator();

				ImGui::Text("Rotation Interpolation");

				ImGui::Text("Initial Spins/sec"); 
				ImGui::DragFloat("  ", &current_emmiter->initial_angular_v, 0.5, 0, 5);

				ImGui::Text("Final Spins/sec");
				ImGui::DragFloat(" ", &current_emmiter->final_angular_v);

				if (ImGui::Button("Apply Rotation"))
				{
					current_emmiter->apply_rotation_interpolation = true; 
					current_emmiter->UpdateRootParticle();
				}
				ImGui::SameLine(); 

				if (ImGui::Button("Stop Applying"))
				{
					current_emmiter->apply_rotation_interpolation = false;
					current_emmiter->UpdateRootParticle();
				}

				ImGui::Separator(); 

				if (ImGui::Button("Save as Template"))
				{

				}
				ImGui::TreePop();
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
		