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
	ImGui_ImplSdlGL2_Init(App->window->window);

	ImGuiIO& io = ImGui::GetIO();

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
	// Manage ImGui Keyboard input

	ImGuiIO& io = ImGui::GetIO();

	//for (int i = 0; i < SCANCODE_AMOUNT; i++)
	//{
	//	if (App->input->GetKey(i) == KEY_DOWN)
	//		io.KeysDown[i] = true; 
	//}

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
		if (ImGui::MenuItem("RandomNum"))
		{
			show_random_number = !show_random_number;
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

		if (ImGui::MenuItem("License"))
		{
			ShellExecute(NULL, "open", "https://github.com/rogerta97/3D_Motor/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
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

	ImGui::EndMainMenuBar(); 

	if (show_gui_demo)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
		ImGui::ShowTestWindow();
	}

	// -------------------------------------------
 
	// Configuration Panel -----------------------

	

	// -------------------------------------------
	
	return UPDATE_CONTINUE;
}

update_status ModuleImGui::PostUpdate(float dt)
{

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
	ImGui::Begin("Console"); 

	for (int i = 0; i < console_buffer.size(); i++)
	{
	
		ImGui::Text(console_buffer[i].c_str());
	}

	ImGui::End(); 

	
}
void ModuleImGui::PrintRandomNumber()
{
	ImGui::Begin("Random Number Generator");
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

	ImGui::End();
}

void ModuleImGui::UpdateConfigPanel()
{
	ImGui::Begin("Configuration");

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
