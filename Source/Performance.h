#pragma once

#ifndef _PERFORMANCE_H_
#define _PERFORMANCE_H_

#include "Timer.h"
#include "ModuleImGui.h"
#include <vector>



class PerformanceWindow
{

public:

	PerformanceWindow() {};

	bool Update(bool &show);

	bool SaveInitData(const char* module); 
	bool SaveRunTimeData(const char* module); 

	bool InitTimer(const char* module);
	bool PauseTimer(const char* module); 
	bool ResumeTimer(const char* module); 

	~PerformanceWindow() {};

private:

	std::vector<float> audio_perf_buffer; 
	std::vector<float> camera_perf_buffer;
	std::vector<float> imgui_perf_buffer;
	std::vector<float> input_perf_buffer;
	std::vector<float> physics_perf_buffer;
	std::vector<float> render_perf_buffer;
	std::vector<float> scene_perf_buffer;
	std::vector<float> window_perf_buffer;

	int audio_perf_init;
	int camera_perf_init;
	int imgui_perf_init;
	int input_perf_init;
	int physics_perf_init;
	int render_perf_init;
	int scene_perf_init;
	int window_perf_init;

public: 

	Timer audio_perf_timer; 
	Timer camera_perf_timer;
	Timer imgui_perf_timer;
	Timer input_perf_timer;
	Timer physics_perf_timer;
	Timer render_perf_timer;
	Timer scene_perf_timer;
	Timer window_perf_timer;

};

#endif