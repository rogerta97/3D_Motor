#include "Application.h"
#include "mmgr\mmgr.h"

#include <stdio.h>

#define HISTOGRAM_FR_LENGHT 25
#define HISTOGRAM_MS_LENGHT 100
#define MAX_MEMORY_LOGGED	50
#define NUM_MODULES 11

Application::Application()
{
	//fps settings
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;

	//file_system = new ModuleFileSystem(true);
	json = new JSON(true);
	window = new ModuleWindow(true);
	input = new ModuleInput(true);
	audio = new ModuleAudio(true);
	scene_intro = new ModuleSceneIntro(true);
	renderer3D = new ModuleRenderer3D(true);
	camera = new ModuleCamera3D(true);
	physics = new ModulePhysics3D(true);
	imgui = new ModuleImGui(true);
	resource_manager = new ResourceLoader(true);


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	//AddModule(file_system);
	AddModule(resource_manager);
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
	json_file*  config = LoadConfig();
	//make sure we have a config
	if (config == nullptr)
		config = CreateDefaultConfig();

	for (std::list<Module*>::iterator item = list_modules.begin(); item != list_modules.end(); item++)
	{
		if (ret == true)
		{
			ret = (*item)->Init(config);
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

	global_timer.Start(); 

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

	memory.push_back(m_getMemoryStatistics().totalActualMemory);

	if (memory.size() > MAX_MEMORY_LOGGED)
		memory.erase(memory.begin());
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
		if (ImGui::Checkbox("Vsync", &App->renderer3D->vsync_on))
		{
			if (SDL_GL_SetSwapInterval(App->renderer3D->vsync_on) < 0)
				LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		}
		ImGui::Text("Framerate AVG: "); ImGui::SameLine();
		int time = global_timer.Read() / 1000;
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.1f", (((float)global_frames / time)));

		char title[25]; 
		sprintf_s(title, 25, "Framerate %.1f", framerate_buffer[framerate_buffer.size() - 1]);
		ImGui::PlotHistogram("##Framerate", &framerate_buffer[0], framerate_buffer.size(), 0, title, 0.0f, 150.0f, ImVec2(300, 100));

		sprintf_s(title, 25, "Miliseconds %.1f", miliseconds_buffer[miliseconds_buffer.size() - 1]);
		ImGui::PlotHistogram("##Frame miliseconds", &miliseconds_buffer[0], miliseconds_buffer.size(), 0, title, 0.0f, 50.0f, ImVec2(300, 100));
		//Memory Application Info
		sMStats stats = m_getMemoryStatistics();

		sprintf_s(title, 25, "Memory %.1f", memory[memory.size() - 1]);
		ImGui::PlotHistogram("##Memory", &memory[0], memory.size(), 0, title, 0.0f, (float)stats.peakReportedMemory * 1.7f, ImVec2(300, 100));
		
		//Hardware
		if (ImGui::TreeNode("Hardware"))
		{
			SDL_version version; 
			SDL_GetVersion(&version);	
			hardware.Start(); 

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

			ImGui::Separator(); 

			ImGui::Text("Total VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.total_video_mem);

			ImGui::Text("Using VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.current_video_mem);

			ImGui::Text("Avaliable VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.available_video_mem);

			ImGui::Text("Reserved VRAM: "); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%.2f", hardware.info.reserved_video_mem);

			{
				char buf[32];
				sprintf(buf, "%.2f/%.2f", hardware.info.current_video_mem, hardware.info.available_video_mem);
				float percentage = (hardware.info.current_video_mem * 100) / hardware.info.available_video_mem;

				ImGui::ProgressBar(percentage / 100, ImVec2(0.f, 0.f), buf);
				ImGui::SameLine(); 
				ImGui::Text("VRAM usage"); 
			}
		

			ImGui::Separator();

			ImGui::Text("Total Reported Mem:"); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.totalReportedMemory);

			ImGui::Text("Total Actual Mem:"); ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.totalActualMemory); 

			ImGui::Text("Peak Reported Mem:"); ImGui::SameLine();  
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.peakReportedMemory); 

			ImGui::Text("Peak Actual Mem: "); ImGui::SameLine();  
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.peakActualMemory); 

			ImGui::Text("Accumulated Reported Mem: "); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.accumulatedReportedMemory); 

			ImGui::Text("Accumulated Actual Mem:"); ImGui::SameLine();  
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.accumulatedActualMemory); 

			ImGui::Text("Accumulated Alloc Unit Count:"); ImGui::SameLine();  
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.accumulatedAllocUnitCount); 

			ImGui::Text("Total Alloc Unit Count:"); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.totalAllocUnitCount); 

			ImGui::Text("Peak Alloc Unit Count:"); ImGui::SameLine(); 
			ImGui::TextColored(ImVec4(1, 1, 0, 1), "%u", stats.peakAllocUnitCount);

			{
				char buf[32];
				sprintf(buf, "%.2f/%.2f", stats.totalReportedMemory, stats.totalActualMemory);
				float percentage = (stats.totalReportedMemory * 100) / stats.totalActualMemory;

				ImGui::ProgressBar(percentage / 100, ImVec2(0.f, 0.f), buf);
			}

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

bool Application::CreateFolder(const char * path) const
{
	bool ret = true;

	if (!CreateDirectory(path, NULL))
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			LOG("Can't create the folder. %s already exist", path);
		}
		else if (GetLastError() == ERROR_PATH_NOT_FOUND)
		{
			LOG("Can't creat folder %s. One or more intermediate directories do not exist", path);
			ret = false;
		}
	}
	return ret;
}

bool Application::CopyFileTo(const char * file, const char * target)
{
	bool ret = true;

	std::string curr_file = file;
	uint cut = curr_file.find_last_of("\\");
	std::string dest_file = target;
	if (dest_file.find_last_of("\\") == dest_file.size() - 1)
		dest_file += curr_file.substr(cut + 1, curr_file.size() - cut + 1);
	else
		dest_file += curr_file.substr(cut, curr_file.size() - cut);

	CopyFile(file, dest_file.c_str(), false);

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

Module* Application::GetModule(int index)
{
	if (index <= NUM_MODULES)
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

void Application::SetState(app_state new_state)
{
	state = new_state; 
}

app_state Application::GetState()
{
	return state;
}

void Application::SendMessageToModules()
{
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); it++)
	{
		(*it)->AdaptToState(state); 
	}
}

void Application::BroadCastEvent(SDL_Event& e)
{
	for (list<Module*>::iterator it = list_modules.begin(); it != list_modules.end(); it++)
	{
		(*it)->BroadCastEvent(e);
	}
}

json_file *  Application::LoadConfig()
{
	return json->LoadJSONFile("config.json");

	if (config != nullptr)
	{
		engine_name = config->GetString("app.engine_name", "No title");
		organization = config->GetString("app.organization", "No org");
		max_fps = config->GetInt("app.max_fps", 60.0f);
		CapFPS(max_fps);

		
		for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); item++)
		{
			(*item)->OnLoadConfig(config);
		}
	}
}
json_file * Application::CreateDefaultConfig()
{
	json_file* config = nullptr;

	config = json->LoadJSONFile("config.json");

	config->SetInt("window.width", 1280);
	config->SetInt("window.height", 980);
	config->SetInt("window.screen_size", 1);
	config->SetInt("window.brightness", 1);
	config->SetString("window.window_mode", "windowed");
	config->SetBool("window.resizable", true);
	config->SetBool("window.borderless", false);

	config->SetString("app.title", "TroEngine");
	config->SetString("app.organization", "UPC CITM");
	config->SetInt("app.max_fps", 0);

	config->SetBool("renderer.vsync", true);

	config->Save();

	return config;
}
void Application::SaveConfig(Module* module)
{
	LOG("Saving actual config....")
	json_file* config = LoadConfig();

	config->SetString("app.organization", organization.c_str());
	config->SetInt("app.max_fps", last_fps);

	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end())
	{
		(*item)->OnSaveConfig(config);
		++item;
	}

	config->Save();
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