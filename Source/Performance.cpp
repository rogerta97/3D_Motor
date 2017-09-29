#include "Performance.h"


bool PerformanceWindow::Update()
{
	ImGui::Begin("Performance");

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
