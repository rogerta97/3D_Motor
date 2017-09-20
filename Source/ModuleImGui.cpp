#include "ModuleImGui.h"
#include "Application.h"

ModuleImGui::ModuleImGui(Application * parent, bool start_enabled) : Module(parent, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{
}

bool ModuleImGui::Init()
{
	ImGui_ImplSdlGL2_Init(App->window->window);

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL2_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::Update(float dt)
{

	// Main menu bar -----------------------------

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Files"))
	{
		if (ImGui::MenuItem("Exit"))
		{
			return UPDATE_STOP;
		}

		if (ImGui::MenuItem("Console"))
		{
			show_console = !show_console;
		}

		ImGui::EndMenu(); 
	}

	if (show_console)
	{
		ImGui::Begin("Console");

		ImGui::End();
	}

	ImGui::EndMainMenuBar(); 

	// -------------------------------------------
 
	ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
	ImGui::ShowTestWindow(&show_test_window);

	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{
	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL2_Shutdown();

	return true;
}
