#pragma once

#include "Globals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "MathGeoLib\MathGeoLib.h"

class Application;
struct PhysBody3D;

class Module
{
private :
	bool enabled;

public:
	Application* App;
	const char* name = ""; 

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init() 
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
};