#include "Application.h"
#include <stdio.h>

#define HISTOGRAM_FR_LENGHT 25
#define HISTOGRAM_MS_LENGHT 100

Application::Application()
{
	json = new JSON(true);
	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	audio = new ModuleAudio(true);
	scene_intro = new ModuleSceneIntro(true);
	renderer3D = new ModuleRenderer3D(true);
	camera = new ModuleCamera3D(true);
	physics = new ModulePhysics3D(true);
	imgui = new ModuleImGui(true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(json);
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);

	// Scenes
	AddModule(scene_intro);

	AddModule(imgui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		delete (*item);

	}
	
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	LoadConfig();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->Init();
			
		}
	
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->Start();

		}
	}
	ms_timer.Start();
	global_timer.Start();
	frame_ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;

	if (miliseconds_buffer.size() > HISTOGRAM_MS_LENGHT)
		miliseconds_buffer.erase(miliseconds_buffer.begin()); 

	miliseconds_buffer.push_back(frame_ms_timer.Read()); 
	frame_ms_timer.Start(); 

	if (ms_timer.Read() > 1000)
	{

		framerate_buffer.push_back(last_sec_frame_counter); 
		last_sec_frame_counter = 0; 

		ms_timer.Start(); 

	}

	if (framerate_buffer.size() > HISTOGRAM_FR_LENGHT)
		framerate_buffer.erase(framerate_buffer.begin()); 
	
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	last_sec_frame_counter++;
	global_frames++; 
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
		
	for(std::list<Module*>::iterator item = list_modules.begin();item!= list_modules.end();item++)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = (*item)->PreUpdate(dt);
		}
	
	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = (*item)->Update(dt);
		}

	}

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == UPDATE_CONTINUE)
		{
			ret = (*item)->PostUpdate(dt);
		}

	}

	FinishUpdate();
	return ret;
}

void Application::PrintConfigData()
{

	if (ImGui::CollapsingHeader("Application"))
	{

		ImGui::InputText("Engine name", (char*)engine_name.c_str(), IM_ARRAYSIZE(engine_name.c_str()));
		window->SetTitle(engine_name.c_str()); 

		ImGui::InputText("Organization", (char*)organization.c_str(), IM_ARRAYSIZE(organization.c_str()));

		ImGui::InputText("MAX FPS", (char*)max_fps.c_str(), IM_ARRAYSIZE(max_fps.c_str()));

		ImGui::Separator(); 

		ImGui::Text("Framerate AVG: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", (global_frames / (global_timer.Read() / 1000.0f)));

		char title[25]; 

		sprintf_s(title, 25, "Framerate %.2f", framerate_buffer.back()); 
		ImGui::PlotHistogram("##Framerate", &framerate_buffer[0], framerate_buffer.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 100));

		sprintf_s(title, 25, "Miliseconds %.2f", miliseconds_buffer.back());
		ImGui::PlotHistogram("##Frame miliseconds", &miliseconds_buffer[0], miliseconds_buffer.size(), 0, title, 0.0f, 50.0f, ImVec2(300, 100));

		if (ImGui::TreeNode("Hardware"))
		{
			SDL_version version; 
			SDL_GetVersion(&version);	

			ImGui::Text("SDL Version"); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d.%d.%d", version.major, version.minor, version.patch); 
			ImGui::Separator();
			ImGui::Text("CPU's:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", SDL_GetCPUCount()); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "(Cache: %dkb)", SDL_GetCPUCacheLineSize());

			ImGui::Text("System RAM:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", SDL_GetSystemRAM());

			ImGui::Text("Caps: "); ImGui::SameLine();

			if (SDL_Has3DNow()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "3DNow,"); ImGui::SameLine();
			if (SDL_HasAVX()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX,"); ImGui::SameLine();
			if (SDL_HasAVX2()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "AVX2,"); ImGui::SameLine();
			if (SDL_HasAltiVec()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "AltiVec,"); ImGui::SameLine();
			if (SDL_HasMMX()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "MMX,"); ImGui::SameLine();
			if (SDL_HasRDTSC()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "RDTSC,");
			if (SDL_HasSSE()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE,"); ImGui::SameLine();
			if (SDL_HasSSE2()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE2,"); ImGui::SameLine();
			if (SDL_HasSSE3()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE3,"); ImGui::SameLine();
			if (SDL_HasSSE41()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE41,"); ImGui::SameLine();
			if (SDL_HasSSE42()) ImGui::TextColored(ImVec4(1, 1, 0, 1), "SSE42,");

			ImGui::Separator(); 
	
			getGraphicsDeviceInfo(&vendorid, &deviceid, &brand, &vm, &vm_curr, &vm_a, &vm_r);

			ImGui::Text("GPU Vendor: "); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer3D->GetGraphicsModel("vendor"));

			ImGui::Text("GPU Model: "); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer3D->GetGraphicsModel("model"));

			ImGui::Text("Total VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", vm);

			ImGui::Text("Using VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", vm_curr);

			ImGui::Text("Avaliable VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", vm_a);

			ImGui::Text("Reserved VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", vm_r);

			ImGui::TreePop(); 
		}		
	}
}

bool Application::CleanUp()
{
	bool ret = true;
	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->CleanUp();
		}
	}

	framerate_buffer.clear(); 
	miliseconds_buffer.clear(); 

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

Module* Application::GetModule(int index)
{
	if (index <= 7)
	{
		int i = 0;

		for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
		{
			if (i == index)
				return (*item);

			i++;
		}
	}

	return nullptr;
	
}
void Application::LoadConfig()
{
	config = json->LoadJSONFile("config.json");

	if (config != nullptr)
	{
		engine_name = config->GetString("app.engine_name", "No title");
		organization = config->GetString("app.organization", "No org");
		max_fps = config->GetString("app.max_fps", "60");

		
		for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
		{
			(*item)->OnLoadConfig(config);
		}
	}
}

void Application::SaveConfig(Module* module)
{
	if (config != nullptr)
	{
		config->SetString("app.title", App->GetAppName());
		config->SetString("app.organization", App->GetOrgName());
		config->SetString("app.max_fps", App->GetMaxFPS());

		for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
		{
			if (module == nullptr)
				(*item)->OnSaveConfig(config);

			else if (module == (*item))
				(*item)->OnSaveConfig(config);
		}

		config->Save();
	}
}
const char * Application::GetAppName()
{
	return engine_name.c_str();
}
const char * Application::GetOrgName()
{
	return organization.c_str();
}
const char * Application::GetMaxFPS()
{
	return max_fps.c_str();
}