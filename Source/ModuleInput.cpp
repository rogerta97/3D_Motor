#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ComponentMaterial.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{

	name = "Input";
	App->performance.InitTimer(name); 

	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	App->performance.SaveInitData(name); 

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	App->performance.InitTimer(name); 

	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_wheel = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{		
		App->imgui->ImGuiInput(&e); 
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_wheel = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			break; 

			case SDL_DROPFILE:	

				std::string file = e.drop.file;

				std::string termination = file.substr(file.size() - 3, 3);

				if (termination == "fbx" || termination == "FBX")
				{
					LOG("FBX file dragged to window", termination); 
					App->scene_intro->ClearGOList(); 
					App->fbx_loader->LoadFBX(file.c_str());

				}				

				if (termination == "png" || termination == "PNG")
				{
					//For now we set the texture in the last obect created (the unique in the scene) 
					LOG("PNG file dragged to window", termination);
					ComponentMaterial* new_material = nullptr; 

					new_material = App->fbx_loader->ImportImage(file.c_str());

					if (App->scene_intro->GetGameObject(0) != nullptr)
					{
						ComponentMaterial* mat_to_change = (ComponentMaterial*)App->scene_intro->GetGameObject(0)->GetComponent(COMPONENT_MATERIAL);

						mat_to_change->Set(new_material); 
					
						delete(new_material); 
					}
				}
					
				break;
		}
	}

	if(quit == true || (keyboard[SDL_SCANCODE_ESCAPE] == KEY_DOWN && keyboard[SDL_SCANCODE_LSHIFT] == KEY_REPEAT))
		return UPDATE_STOP;

	App->performance.SaveRunTimeData(name); 

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::PrintConfigData()
{
	ImGuiIO& io = ImGui::GetIO();

	mouse_in_window = io.WantCaptureMouse;
	
	if (ImGui::CollapsingHeader(name))
	{
		ImGui::Text("Mouse Position:");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "X: %d", App->input->mouse_x);	
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Y: %d", App->input->mouse_y);
		ImGui::Text("Mouse Motion:");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "X: %d", App->input->mouse_x_motion);

		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Y: %d", App->input->mouse_y_motion);

		ImGui::Text("Mouse Wheel:");
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "State: %d", App->input->mouse_wheel);
		
		ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d (%.02f secs)", i, io.KeysDownDuration[i]); }
		ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", i); }
		ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%d", i); }
		ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
	}
	
}