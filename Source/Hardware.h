#pragma once

#include "DeviceId/DeviceId.h"
#include <string>

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

struct HardwareInfo 
{
	std::string vendor; 
	std::string brand;

	float total_video_mem = -1.0f;
	float current_video_mem = -1.0f;
	float available_video_mem = -1.0f;
	float reserved_video_mem = -1.0f;
};

class Hardware
{
public:

	bool GetHardwareData(std::string* vendor, std::string* deviceid, std::string* brand, float* tot_vm, float* curr_vm, float* av_vm, float* res_vm);

private: 

	HardwareInfo info; 
	
};

#endif // !_HARDWARE_H_

