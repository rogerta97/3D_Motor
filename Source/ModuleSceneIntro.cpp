#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "imgui.h"

#include <stdlib.h>
#include <time.h>

#define RADIUS 44

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	srand(time(NULL));

	//App->audio->PlayMusic("F-Zero_-_Mute_City.ogg");

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = 1920.0f;
	io.DisplaySize.y = 1280.0f;
	io.RenderDrawListsFn = NULL;

	//unsigned char* pixels;
	//int width, height;
	//io.Fonts->GetTexDataAsRGBA32(pixels, &width, &height);

	
	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	/*Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();*/

	for (p2List_item<Cylinder>* aux = Cylinders.getFirst(); aux != nullptr; aux = aux->next) {
		aux->data.Render();
	}

	// ImGui Content ------------

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;
	io.MousePos = ImVec2(App->input->GetMouseX(), App->input->GetMouseY());

	io.MouseDown[0] = App->input->GetKey(0);
	io.MouseDown[1] = App->input->GetKey(1);

	ImGuiUpdate(); 

	// -------------------------


	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::ImGuiUpdate()
{
	
}

