#include "ModuleImGui.h"
#include "Application.h"
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

	show_console = true;

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
			App->OpenWebPage("https://github.com/rogerta97/3D_Motor/blob/master/LICENSE");
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
	if (show_performance) App->performance.Update(show_performance);

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

	ImGui::Render();

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

void ModuleImGui::ImGuiInput(SDL_Event* e)
{
	ImGui_ImplSdlGL2_ProcessEvent(e);
}
