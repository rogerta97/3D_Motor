#include "Application.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	imgui = new ModuleImGui(this); 

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
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
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init();
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;

	if (ms_timer.Read() > 1000)
	{
		if (framerate_buffer.size() > 100)
			framerate_buffer.pop();

		if (miliseconds_buffer.size() > 100)
			miliseconds_buffer.pop();

		framerate_buffer.push(frame_counter);
		miliseconds_buffer.push(ms_timer.Read()); 

		frame_counter = 0; 
		ms_timer.Start();
	}
	
	frame_counter++;
}

// ---------------------------------------------
void Application::FinishUpdate()
{

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	p2List_item<Module*>* item = list_modules.getFirst();
	
	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PreUpdate(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->Update(dt);
		item = item->next;
	}

	item = list_modules.getFirst();

	while(item != NULL && ret == UPDATE_CONTINUE)
	{
		ret = item->data->PostUpdate(dt);
		item = item->next;
	}

	FinishUpdate();
	return ret;
}

void Application::PrintConfigData()
{

	if (ImGui::CollapsingHeader("Application"))
	{

		char buf[32] = "";
		ImGui::InputText("Engine name", buf, IM_ARRAYSIZE(buf));
		ImGui::InputText("Organization", buf, IM_ARRAYSIZE(buf));
		ImGui::InputText("MAX FPS", buf, IM_ARRAYSIZE(buf));

		ImGui::Text("Framerate: "); ImGui::NewLine(); 

		//ImGui::PlotHistogram("##Framerate", framerate_buffer.front(), framerate_buffer.size(), 0, "klk", 0.0f, 100.0f, ImVec2(310, 10));

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

			ImGui::TreePop(); 
		}
		
	}

}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

Module* Application::GetModule(int index)
{
	return list_modules.get(index);
}
