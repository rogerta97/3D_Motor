#include "Performance.h"

#define HISTOGRAM_FR_LENGHT 25

bool PerformanceWindow::Update(bool& show_window)
{
	ImGui::Begin("Performance", &show_window);

	ImGui::Text("START-UP PERFORMANCE:"); ImGui::NewLine(); 

	ImGui::Text("Window Initialization: %d ms", window_perf_init); ImGui::NewLine(); 
	ImGui::Text("Camera Initialization: %d ms", camera_perf_init); ImGui::NewLine();
	ImGui::Text("Input Initialization: %d ms", input_perf_init); ImGui::NewLine();
	ImGui::Text("Editor Initialization: %d ms", imgui_perf_init); ImGui::NewLine();
	ImGui::Text("Render Initialization: %d ms", render_perf_init); ImGui::NewLine();
	ImGui::Text("Audio Initialization: %d ms", audio_perf_init); ImGui::NewLine();
	ImGui::Text("Scene Initialization: %d ms", scene_perf_init); ImGui::NewLine();
	ImGui::Text("Physics Initialization: %d ms", physics_perf_init); ImGui::NewLine();

	ImGui::Separator(); 
	ImGui::Separator();

	ImGui::Text("RUN TIME PERFORMANCE:"); ImGui::NewLine(); 

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "ONLY MODULES THAT UPDATE ITS PROPERTIES WILL BE SHOWN HERE"); ImGui::NewLine();

	ImGui::PlotLines("", &camera_perf_buffer[0], camera_perf_buffer.size(), 0, "Camera", 0.0f, 5.0f, ImVec2(300,100));
	ImGui::PlotLines("", &imgui_perf_buffer[0], imgui_perf_buffer.size(), 0, "ImGui", 0.0f, 5.0f, ImVec2(300, 100));
	ImGui::PlotLines("", &input_perf_buffer[0], input_perf_buffer.size(), 0, "Input", 0.0f, 5.0f, ImVec2(300, 100));
	ImGui::PlotLines("", &physics_perf_buffer[0], physics_perf_buffer.size(), 0, "Physics", 0.0f, 5.0f, ImVec2(300, 100));
	ImGui::PlotLines("", &render_perf_buffer[0], render_perf_buffer.size(), 0, "Render", 0.0f, 5.0f, ImVec2(300, 100));
	
	ImGui::End(); 

	return true; 
}

bool PerformanceWindow::SaveInitData(const char * module)
{
	if (module == "Audio") audio_perf_init = audio_perf_timer.Read(); 
	if (module == "Camera") camera_perf_init = camera_perf_timer.Read();
	if (module == "ImGui") imgui_perf_init = imgui_perf_timer.Read(); 
	if (module == "Input") input_perf_init = input_perf_timer.Read();
	if (module == "Physics") physics_perf_init = physics_perf_timer.Read();
	if (module == "Render") render_perf_init = render_perf_timer.Read();
	if (module == "Scene") scene_perf_init = scene_perf_timer.Read();
	if (module == "Window") window_perf_init = window_perf_timer.Read();
		
	return true;
}

bool PerformanceWindow::SaveRunTimeData(const char * module)
{

	std::vector<float>* curr_list; 

	if (module == "Audio") curr_list = &audio_perf_buffer; 
	if (module == "Camera") curr_list = &camera_perf_buffer; 
	if (module == "ImGui") curr_list = &imgui_perf_buffer;
	if (module == "Input") curr_list = &input_perf_buffer;
	if (module == "Physics") curr_list = &physics_perf_buffer;
	if (module == "Render") curr_list = &render_perf_buffer;
	if (module == "Scene") curr_list = &scene_perf_buffer;
	if (module == "Window") curr_list = &window_perf_buffer;

	if (curr_list->size() > HISTOGRAM_FR_LENGHT)
		curr_list->erase(curr_list->begin()); 

	if (module == "Audio") 
	{
		curr_list->push_back(audio_perf_timer.Read()); audio_perf_timer.Start();
	}

	if (module == "Camera") 
	{
		curr_list->push_back(camera_perf_timer.Read()); camera_perf_timer.Start();
	}

	if (module == "ImGui") 
	{
		curr_list->push_back(imgui_perf_timer.Read()); imgui_perf_timer.Start();
	}

	if (module == "Input") 
	{
		curr_list->push_back(input_perf_timer.Read()); input_perf_timer.Start();
	}

	if (module == "Physics") 
	{
		curr_list->push_back(physics_perf_timer.Read()); physics_perf_timer.Start();
	}

	if (module == "Render") 
	{
		curr_list->push_back(render_perf_timer.Read()); render_perf_timer.Start();
	}

	if (module == "Scene") 
	{
		curr_list->push_back(scene_perf_timer.Read()); scene_perf_timer.Start();
	}

	if (module == "Window") 
	{
		curr_list->push_back(window_perf_timer.Read()); window_perf_timer.Start();
	}

	return true;
}

bool PerformanceWindow::InitTimer(const char * module)
{
	if (module == "Audio") audio_perf_timer.Start();
	if (module == "Camera") camera_perf_timer.Start();
	if (module == "ImGui") imgui_perf_timer.Start();
	if (module == "Input") input_perf_timer.Start();
	if (module == "Physics") physics_perf_timer.Start();
	if (module == "Render") render_perf_timer.Start();
	if (module == "Scene") scene_perf_timer.Start();
	if (module == "Window") window_perf_timer.Start();

	return true;
}

bool PerformanceWindow::PauseTimer(const char * module)
{
	if (module == "Audio") audio_perf_timer.Pause();
	if (module == "Camera") camera_perf_timer.Pause();
	if (module == "ImGui") imgui_perf_timer.Pause();
	if (module == "Input") input_perf_timer.Pause();
	if (module == "Physics") physics_perf_timer.Pause();
	if (module == "Render") render_perf_timer.Pause();
	if (module == "Scene") scene_perf_timer.Pause();
	if (module == "Window") window_perf_timer.Pause();

	return true; 
}

bool PerformanceWindow::ResumeTimer(const char * module)
{
	if (module == "Audio") audio_perf_timer.Resume();
	if (module == "Camera") camera_perf_timer.Resume();
	if (module == "ImGui") imgui_perf_timer.Resume();
	if (module == "Input") input_perf_timer.Resume();
	if (module == "Physics") physics_perf_timer.Resume();
	if (module == "Render") render_perf_timer.Resume();
	if (module == "Scene") scene_perf_timer.Resume();
	if (module == "Window") window_perf_timer.Resume();

	return true; 
}
