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

	Hardware();

	void Start(); 
	bool GetHardwareData();

	~Hardware();
	
	HardwareInfo info;
	
};

#endif // !_HARDWARE_H_

