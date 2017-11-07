#pragma once

#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "MathGeoLib\MathGeoLib.h"

class Application;
struct PhysBody3D;
class json_file;
class Module
{
private :
	bool enabled;

public:
	const char* name = ""; 

	Module(bool start_enabled = true)
	{}

	virtual ~Module()
	{}

	virtual bool Init(json_file* config) 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void PrintConfigData()
	{
		
	}

	virtual void OnCollision(PhysBody3D* body1, PhysBody3D* body2)
	{}
	virtual void OnLoadConfig(json_file* config) {};
	virtual void OnSaveConfig(json_file* config) {};
};