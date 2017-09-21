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

	name = "ImGui"; 

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

		ImGui::EndMenu(); 
	}

	if (ImGui::BeginMenu("Tools"))
	{
		if (ImGui::MenuItem("Console"))
		{
			show_console = !show_console;
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
			ShellExecute(NULL, "open", "https://github.com/rogerta97/3D_Motor", NULL, NULL, SW_SHOWNORMAL);
		}

		if (ImGui::MenuItem("Download latest"))
		{
			ShellExecute(NULL, "open", "https://github.com/rogerta97/3D_Motor/releases", NULL, NULL, SW_SHOWNORMAL);
		}

		if (ImGui::MenuItem("Report a bug"))
		{
			ShellExecute(NULL, "open", "http://yourwebpage.com", NULL, NULL, SW_SHOWNORMAL);
		}

		if (ImGui::MenuItem("About"))
		{
			show_about = !show_about; 
		}

		ImGui::EndMenu();
	}

	if (show_console) PrintConsole();	
	if (show_about) ShowAbout(); 

	ImGui::EndMainMenuBar(); 

	if (show_gui_demo)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowTestWindow();
	}

	// -------------------------------------------
 
	// Configuration Panel -----------------------

	UpdateConfigPanel(); 

	// -------------------------------------------
	
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

void ModuleImGui::AddToConsole(const char * new_line)
{
	console_buffer.PushBack(new_line); 
}

void ModuleImGui::ClearConsole()
{
	console_buffer.Clear(); 
}

void ModuleImGui::PrintConsole()
{
	ImGui::Begin("Console"); 

	for (int i = 0; i < console_buffer.Count(); i++)
	{
		ImGui::Text(console_buffer[i]);
	}

	ImGui::End(); 

	console_buffer.Clear();
}

void ModuleImGui::ShowAbout()
{
	ImGui::Begin("About");

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
	ImGui::TextWrapped("");
	ImGui::TextWrapped("MIT License:");

	ImGui::End();
}

void ModuleImGui::UpdateConfigPanel()
{
	ImGui::Begin("Configuration");

	Module* curr_module = App->GetModule(0);

	for (int i = 1; App->GetModule(i) != nullptr; i++)
	{
		ImGui::CollapsingHeader(App->GetModule(i)->name);
		App->GetModule(i)->PrintConfigData(); 
	}
	
	ImGui::End();
}
