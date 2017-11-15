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

bool ModuleImGui::Init(json_file* config)
{
	name = "ImGui";
	App->performance.InitTimer(name);

	ImGui_ImplSdlGL2_Init(App->window->window);

	SetInitColors();

	panel_octree.Start(); 
	
	console.SetBoolean(true); 

	App->performance.SaveInitData(name);

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);
	ImGuizmo::BeginFrame();
	ImGuizmo::Enable(true);
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	App->performance.InitTimer(name); 
	// Manage ImGui Keyboard input
	//Change guizmo current operation
	if (App->scene_intro->GetCurrentGO() != nullptr)
	{
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			curr_operation = ImGuizmo::OPERATION::TRANSLATE;
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
			curr_operation = ImGuizmo::OPERATION::ROTATE;
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
			curr_operation = ImGuizmo::OPERATION::SCALE;
	}
	// Main menu bar -----------------------------

	if (PrintTopBar() != UPDATE_CONTINUE)
		return UPDATE_STOP; 

	// Update panels -----------------------------


	//inspector.Draw(); 
	//console.PrintConsole(); 

	App->performance.SaveRunTimeData(name); 
	
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	App->renderer3D->lights[0].on = false; 
	
	UpdateConfigPanel();
	// TODO draw all the other things before

	App->renderer3D->UI_attributes();

	inspector.Draw();
	hierarchy.Draw(); 
	console.PrintConsole(); 
	raycast.Update();

	ImGui::Render();
	App->renderer3D->CustomAttributes();

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL2_Shutdown();

	return true;
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

void ModuleImGui::PrintStyleEditor()
{
	ImGui::Begin("Style", &show_style_editor);

	ImGui::ShowStyleEditor(); 

	ImGui::End();
}

void ModuleImGui::AddInConsole(const std::string new_str)
{
	console.AddToConsole(new_str); 
}

update_status ModuleImGui::PrintTopBar()
{

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
			console.SetBoolean(true);
		}
		if (ImGui::MenuItem("RandomNum"))
		{
			show_random_number = !show_random_number;
		}

		if (ImGui::MenuItem("Performance"))
		{
			show_performance = !show_performance;
		}

		if (ImGui::MenuItem("Octree"))
		{
			panel_octree.Start();
			show_octree_window = !show_octree_window;
		}

		ImGui::Separator();

		if (ImGui::MenuItem("Configuration"))
		{
			show_configuration = !show_configuration;
		}

		if (ImGui::MenuItem("Style"))
		{
			show_style_editor = !show_style_editor;
		}

		ImGui::EndMenu();
	}


	if (ImGui::BeginMenu("Create"))
	{
		if (ImGui::BeginMenu("GameObject"))
		{

			if (ImGui::MenuItem("Empty", NULL))			
				App->scene_intro->CreateGameObject("GameObject"); 
			
			if (ImGui::MenuItem("Cube"))
			{
				GameObject* cube = App->scene_intro->CreateGameObject("Cube");

				//Check name collisions in hierarchy 
	
				string new_name("Cube "); 
				new_name += to_string(App->scene_intro->GetList().size());
				cube->SetName(new_name.c_str()); 
									
				ComponentMeshRenderer* n_mr = new ComponentMeshRenderer(cube);				
				n_mr->SetCubeVertices({ 0,0,0 }, 2);
				cube->PushComponent(n_mr);

				ComponentTransform* trans = (ComponentTransform*)cube->GetComponent(COMPONENT_TRANSFORM);
				trans->SetLocalPosition({ 0,0,0 });
											
			}
				
			ImGui::EndMenu(); 
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

	if (show_random_number)PrintRandomNumber();
	if (show_about) ShowAbout();
	if (show_performance) App->performance.Update(show_performance);
	if (show_style_editor) PrintStyleEditor();
	if (show_octree_window)
	{
	
		panel_octree.DrawPanel();
	}

	if (App->scene_intro->IsSceneEmpty() == false)
		show_inspector = true;
	else
		show_inspector = false;

	ImGui::EndMainMenuBar();

	if (show_gui_demo)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowTestWindow();
	}

	return UPDATE_CONTINUE; 
}

void ModuleImGui::ImGuiInput(SDL_Event* e)const
{
	ImGui_ImplSdlGL2_ProcessEvent(e);
}

void ModuleImGui::SetInitColors()
{
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
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.11f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.11f, 0.44f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.40f, 0.32f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.09f, 0.38f, 0.80f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.32f, 0.61f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.44f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.48f, 0.78f, 0.60f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.96f, 0.31f, 0.04f, 0.45f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.96f, 0.31f, 0.04f, 0.45f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.21f, 0.17f, 0.69f, 0.45f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.19f, 0.37f, 0.75f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.19f, 0.37f, 0.75f, 0.80f);
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
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.94f, 1.00f, 0.41f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.32f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.96f, 0.93f, 0.07f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	style.ItemSpacing.y = 6;
	style.GrabRounding = 3;
}
	


