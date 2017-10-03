#include "Application.h"
#include <stdio.h>

#define HISTOGRAM_FR_LENGHT 25
#define HISTOGRAM_MS_LENGHT 100
Application::Application()
{
	//fps settings
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

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

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	++fps_counter;
	++global_frames;
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{

	//update last fps
	if (fps_timer.Read() > 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}
	//update last ms
	last_frame_ms = ms_timer.Read();
	//Delay for cap fps
	DelayToCap();
		//fit the histogram erasing the first and adding a new balue in the next iteration
	//Update the buffers to get the new frame information
	framerate_buffer.push_back(last_fps);
	FitHistogram();
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
	miliseconds_buffer.push_back(ms_timer.Read());

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

		if (ImGui::SliderFloat("Max FPS", &max_fps, 0, 120))
			App->CapFPS(max_fps);
	
		ImGui::Separator(); 

		ImGui::Text("Framerate AVG: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", (global_frames / (ms_timer.Read()) / 1000.0f));

		char title[25]; 
		sprintf_s(title, 25, "Framerate %.1f", framerate_buffer[framerate_buffer.size() - 1]);
		ImGui::PlotHistogram("##Framerate", &framerate_buffer[0], framerate_buffer.size(), 0, title, 0.0f, 100.0f, ImVec2(300, 100));

		
		sprintf_s(title, 25, "Miliseconds %.1f", miliseconds_buffer[miliseconds_buffer.size() - 1]);
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

			ImGui::Text("GPU Vendor: "); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer3D->GetGraphicsModel("vendor"));

			ImGui::Text("GPU Model: "); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", renderer3D->GetGraphicsModel("model"));

			ImGui::Text("Total VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.total_video_mem);

			ImGui::Text("Using VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.current_video_mem);

			ImGui::Text("Avaliable VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.available_video_mem);

			ImGui::Text("Reserved VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.reserved_video_mem);

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
		CapFPS(config->GetNumber("app.max_fps", 60.0f));

		
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
		config->SetNumber("app.max_fps", App->GetCapFPS());

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


void Application::OpenWebPage(const char * url)
{
	ShellExecute(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

void Application::DelayToCap() const
{
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		SDL_Delay(capped_ms - last_frame_ms);
	}
}



void Application::FitHistogram()
{
	if (framerate_buffer.size() > HISTOGRAM_FR_LENGHT)
		framerate_buffer.erase(framerate_buffer.begin());

	if (miliseconds_buffer.size() > HISTOGRAM_MS_LENGHT)
		miliseconds_buffer.erase(miliseconds_buffer.begin());
}

uint Application::GetCapFPS() const
{
	if (capped_ms > 0)
		return (uint)((1.0f / (float)capped_ms) * 1000.0f);
	else
		return 0;//math error avoiding
}

void Application::CapFPS(float fps)
{
	if (fps > 0)
	{
		capped_ms = (1000 / fps);
		LOG("FPS cap: %.3f", fps);
	}
	else 
	{
		capped_ms = 0;
	}
}