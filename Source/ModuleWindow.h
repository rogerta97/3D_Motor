#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(json_file* config);
	void PrintConfigData(); 
	bool CleanUp();

	void SetTitle(const char* title);
	void OnLoadConfig(json_file * config);
	void OnSaveConfig(json_file * config);

	void BroadCastEvent(SDL_Event& e);


public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	//Windows config
	int width = 0;
	int height = 0; 

	bool fullscreen = false; 
	bool borderless = false; 
	
};

#endif // __ModuleWindow_H__