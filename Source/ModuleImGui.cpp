#include "ModuleImGui.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentDefs.h"
#include <ctime>

#define SCANCODE_AMOUNT 284

ModuleImGui::ModuleImGui(bool start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{
	name = "ImGui";
	App->performance.InitTimer(name);

	ImGui_ImplSdlGL2_Init(App->window->window);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.40f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.68f, 0.44f, 0.44f, 0.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.30f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.41f, 0.05f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.41f, 0.05f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.41f, 0.05f, 0.01f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.62f, 0.21f, 0.21f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.72f, 0.65f, 0.21f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.70f, 0.03f, 0.03f, 0.40f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.67f, 0.40f, 0.40f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.67f, 0.40f, 0.40f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.72f, 0.08f, 0.08f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 0.14f, 0.14f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.21f, 0.29f, 0.80f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.90f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.70f, 0.70f, 0.90f, 0.60f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.86f, 0.02f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.86f, 0.02f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.96f, 0.93f, 0.07f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	App->performance.SaveInitData(name);

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	App->performance.InitTimer(name); 
	// Manage ImGui Keyboard input

	ImGuiIO& io = ImGui::GetIO();

	// Main menu bar -----------------------------

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Files"))
	{
		if (ImGui::MenuItem("Exit", "Shift + Esc"))
		{
			return UPDATE_STOP;
		}

		ImGui::EndMenu(); 
	}

	if (ImGui::BeginMenu("Tools"))
	{
		if (ImGui::MenuItem("Console"))
		{
			show_console = !show_console;
		}
		if (ImGui::MenuItem("RandomNum"))
		{
			show_random_number = !show_random_number;
		}

		if (ImGui::MenuItem("Configuration"))
		{
			show_configuration = !show_configuration;
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Performance"))
		{
			show_performance = !show_performance;
		}

		ImGui::EndMenu();		
	}

	if (ImGui::BeginMenu("Help"))
	{

		if (ImGui::MenuItem("DEMO GUI"))
		{
			show_gui_demo = !show_gui_demo;
		}

		if (ImGui::MenuItem("Documentation"))
		{
			App->OpenWebPage("https://github.com/rogerta97/3D_Motor");
		}

		if (ImGui::MenuItem("Download latest"))
		{
			App->OpenWebPage("https://github.com/rogerta97/3D_Motor/releases");
		}

		if (ImGui::MenuItem("Report a bug"))
		{
			App->OpenWebPage("https://github.com/rogerta97/3D_Motor/issues");
		}

		if (ImGui::MenuItem("License"))
		{
			App->OpenWebPage("https://github.com/rogerta97/3D_Motor/blob/master/LICENSE");			
		}

		if (ImGui::MenuItem("About"))
		{
			show_about = !show_about; 
		}

		ImGui::EndMenu();
	}

	if (show_console) PrintConsole();
	if (show_random_number)PrintRandomNumber();
	if (show_about) ShowAbout();
	if (show_inspector)PrintInspector();
	if (show_performance) App->performance.Update(show_performance);
	if (show_hierarchy) PrintHierarchy();

	if (App->scene_intro->IsListEmpty() == false)
		show_inspector = true; 
	else 
		show_inspector = false; 

	ImGui::EndMainMenuBar(); 

	if (show_gui_demo)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowTestWindow();
	}

	// -------------------------------------------
 
	App->performance.SaveRunTimeData(name); 
	
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	App->renderer3D->lights[0].on = false; 
	
	UpdateConfigPanel();
	// TODO draw all the other things before
	App->renderer3D->UI_attributes();
	ImGui::Render();
	App->renderer3D->CustomAttributes();


	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL2_Shutdown();
	ClearConsole(); 

	return true;
}

void ModuleImGui::AddToConsole(std::string new_line)
{
	console_buffer.push_back(new_line); 
}

void ModuleImGui::ClearConsole()
{
	console_buffer.clear(); 
}

void ModuleImGui::PrintConsole()
{
	ImGui::Begin("Console", &show_console); 

	for (int i = 0; i < console_buffer.size(); i++)
	{	
		ImGui::Text(console_buffer[i].c_str()); 
	}

	ImGui::End(); 

	
}
void ModuleImGui::PrintProperties() 
{
	//if any gizmo has been created

	GameObject* curr_GO = nullptr;// = App->fbx_loader->; // App->set the dragged gizmo
	//ImGui::SetNextWindowPos(ImVec2((float)properties_panel_x, (float)properties_panel_y), ImGuiSetCond_Always);
	//ImGui::SetNextWindowSize(ImVec2((float)properties_panel_w, (float)properties_panel_h), ImGuiSetCond_Always);
	/*ImGui::Begin("Properties", &show_properties);

	ComponentTransform* trans_tmp = (ComponentTransform*)App->scene_intro->GetGameObject(0)->GetComponent(COMPONENT_TRANSFORM);
	
	if (curr_GO != nullptr)
	{	 
		float3 pos = trans_tmp->GetPosition();
		float3 rot = trans_tmp->GetRotation();
		float3 scale = trans_tmp->GetScale();

		if (ImGui::DragFloat3("Position", (float*)&pos, 0.25f))
			trans_tmp->SetPosition(pos);

		if (ImGui::DragFloat3("Rotation", (float*)&rot))
			trans_tmp->SetRotation(rot);

		if (ImGui::DragFloat3("Scale", (float*)&scale, 0.05f))
			trans_tmp->SetScale(scale);

		ImGui::Text("Current Triangles : %d", trans_tmp->GetTriNum());
		ImGui::Text("Texture size %d", trans_tmp->GetTexSize());
	}*/
	ImGui::End();
}
void ModuleImGui::PrintRandomNumber()
{
	ImGui::Begin("Random Number Generator", &show_random_number);
	ImGui::Text("Random Generator tool:");
	ImGui::Text("-Random Integer between the 2 values:");
	ImGui::DragInt("Minimum", &i_min, 1,0, 100);
	ImGui::DragInt("Maximum", &i_max, 1, 0, 100);
	if (ImGui::Button("Generate Int") == true)
	{
		i_rand = (rand() % (i_max - i_min)) + i_min;
	}
	ImGui::Text("%d", i_rand);
	ImGui::Text("Random Float Generator:");
	if (ImGui::Button("Generate float") == true)
	{
		//std function
		f_rand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
	ImGui::Text("%f", f_rand);


	ImGui::End();
}
void ModuleImGui::ShowAbout()
{
	ImGui::Begin("About", &show_about);

	ImGui::TextWrapped("3D Engine");
	ImGui::TextWrapped("We are two students from UPC, Barcelona. We are developing a 3D Open Engine for developers to make videogames. Providing some useful tools!");
	ImGui::TextWrapped("");
	ImGui::TextWrapped("Authors:");	
	ImGui::TextWrapped(" Pau Bonet");
	ImGui::TextWrapped(" Roger Tello");
	ImGui::TextWrapped("");
	ImGui::TextWrapped("Libraries:");
	ImGui::TextWrapped(" MathGeoLib");
	ImGui::TextWrapped(" STL");
	ImGui::TextWrapped(" Bullet");
	ImGui::TextWrapped(" ImGui");
	ImGui::TextWrapped(" DevIL");
	ImGui::TextWrapped(" Assimp");



	ImGui::End();
}

void ModuleImGui::UpdateConfigPanel()
{
	if (!show_configuration)
		return;

	ImGui::Begin("Configuration", &show_configuration);

	App->PrintConfigData();

	for (int i = 0; App->GetModule(i) != nullptr; i++)
	{
		App->GetModule(i)->PrintConfigData(); 
	}
	ImGui::End();
}

void ModuleImGui::ImGuiInput(SDL_Event* e)const
{
	ImGui_ImplSdlGL2_ProcessEvent(e);
}
void ModuleImGui::PrintInspector()
{
	if (ImGui::Begin("Inspector", &show_inspector))
	{
		for (int i = 0; i < App->scene_intro->GetList().size(); i++)
		{
			GameObject* g_aux = App->scene_intro->GetGameObject(i);
			//if (g_aux->Active() == true)
			//{
			ComponentTransform* ct_aux = (ComponentTransform*)g_aux->GetComponent(COMPONENT_TRANSFORM);
			//TRANSFORM
			if (ImGui::TreeNode(g_aux->GetName()))
			{
				if (ImGui::TreeNode("Transform"))
				{
					int j = 0; 
					while (App->scene_intro->GetGameObject(i)->GetNumMeshes() > j)
					{
						string tree_name("Mesh "); 	
						tree_name += to_string(j + 1); 
						
						if (ImGui::TreeNode(tree_name.c_str()))
						{
							ComponentTransform* ct_aux = (ComponentTransform*)g_aux->GetComponent(COMPONENT_TRANSFORM, j);

							float3 radians_angle = ct_aux->GetLocalRotation()->ToEulerXYZ();

							float pos[3] = { ct_aux->GetLocalPosition()->x,ct_aux->GetLocalPosition()->y,ct_aux->GetLocalPosition()->z };
							float rot[3] = { RadToDeg(radians_angle.x),RadToDeg(radians_angle.y),RadToDeg(radians_angle.z), };
							float s[3] = { ct_aux->GetLocalScale()->x,ct_aux->GetLocalScale()->y,ct_aux->GetLocalScale()->z };

							ImGui::InputFloat3("Pos##transform", pos, 2);
							ImGui::InputFloat3("Rot##transform", rot, 2);
							ImGui::InputFloat3("Scale##transform", s, 2);

							Quat new_quad;
							//TODO
							//Set the variable new quad to the new rotation quaternion from rot vector, then uncoment the line above
							
							ct_aux->SetPosition(float3(pos[0], pos[1], pos[2])); 
							//ct_aux->SetRotation(new_quad);
							ct_aux->SetScale(float3(s[0], s[1], s[2]));

						
							ImGui::TreePop(); 
						}

						j++; 
					}			
					ImGui::TreePop();
				}
				//MESHES
				if (ImGui::TreeNode("Meshes"))
				{
					int j = 0;
					if (App->scene_intro->GetGameObject(i) != nullptr)
					{
						ComponentMeshRenderer* cm_aux = (ComponentMeshRenderer*)g_aux->GetComponent(COMPONENT_MESH_RENDERER);

						char title[25];
						sprintf_s(title, 25, "Mesh %d##meshrenderer", j + 1);
						if (ImGui::TreeNode(title))
						{
							char name[25];
							sprintf_s(name, 25, "Vertices##mesh%d", j + 1);
							ImGui::LabelText(name, "%d", cm_aux->num_vertices);
							sprintf_s(name, 25, "Indices##mesh%d", j + 1);
							ImGui::LabelText(name, "%d", cm_aux->num_indices);
							ImGui::TreePop();
						}
						++j;
						if (App->scene_intro->GetGameObject(i) == nullptr)
							break;
					}
					ImGui::TreePop();
				}
				//MATERIALS
				if (ImGui::TreeNode("Materials"))
				{
					ComponentMaterial* cma_aux = (ComponentMaterial*)g_aux->GetComponent(COMPONENT_MATERIAL);
					ImGui::LabelText("ID##material", "%d", cma_aux->textures_id);
					ImGui::LabelText("Width##material", "%d", cma_aux->width);
					ImGui::LabelText("Height##material", "%d", cma_aux->height);
					ImGui::LabelText("Path##material", "%s", cma_aux->path.c_str());
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
			//}//activ
		}
	}
	ImGui::End();
}
	

void ModuleImGui::PrintHierarchy()
{

	if (ImGui::Begin("Hierarchy"), &show_hierarchy && App->scene_intro->IsListEmpty() == false)
	{	
		for (int i = 0; i < App->scene_intro->GetList().size(); i++)
		{
			if (ImGui::TreeNode(App->scene_intro->GetList()[i]->GetName()))
			{
				GameObject* aux = App->scene_intro->GetList()[i];
				for (int j = 0; j < aux->GetNumMeshes(); j++)
				{
					//ComponentMeshRenderer* cm_aux = (ComponentMeshRenderer*)aux->GetComponent(COMPONENT_MESH_RENDERER, j);
					if (ImGui::TreeNode("Mesh"))
					{
						ImGui::LabelText("ID mesh", "%d", j);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
		
		
	}	

	ImGui::End();
}

