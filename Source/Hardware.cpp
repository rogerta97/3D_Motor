#include "Hardware.h"

Hardware::Hardware()
{
	GetHardwareData(); 
}

bool Hardware::GetHardwareData()
{
	uint64_t tot_vm, curr_vm, av_vm, res_vm; 

	if (getGraphicsDeviceInfo(nullptr, nullptr, nullptr, &tot_vm, &curr_vm, &av_vm, &res_vm))
	{

		info.total_video_mem = (float)tot_vm / (1024.0f * 1024.0f);
		info.current_video_mem = (float)curr_vm / (1024.0f * 1024.0f);
		info.available_video_mem = (float)av_vm / (1024.0f * 1024.0f);
		info.reserved_video_mem = (float)res_vm / (1024.0f * 1024.0f);

		return true;
	}

	return false;
}

Hardware::~Hardware()
{
}
