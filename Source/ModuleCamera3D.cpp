#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleFBXLoader.h"
#include "ModuleSceneIntro.h"
#include "Gizmo.h"

#define STD_CAM_DISTANCE 10
ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Init() 
{
	name = "Camera";
	App->performance.InitTimer(name);

	App->performance.SaveInitData(name);

	return true; 
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	Gizmo* aux;
	if (App->scene_intro->obj_list.empty() == false)
		 aux = App->scene_intro->obj_list.back();
	else
		 aux = nullptr;
	App->performance.InitTimer(name); 

	vec3 newPos(0,0,0);
	
	float temporal_speed = mov_speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		temporal_speed=temporal_speed*0.3f;
	

	//if(App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += temporal_speed;
	//if(App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= temporal_speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z *temporal_speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z *temporal_speed;

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * temporal_speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * temporal_speed;
	
	if(App->input->GetMouseWheel() == 1) newPos -= Z *temporal_speed*mouse_wheel_speed;
	if (App->input->GetMouseWheel() == -1) newPos += Z *temporal_speed*mouse_wheel_speed;


	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT)
			//TODO put the last gizmo position to orbit arround it
		{
			if (aux == nullptr)
				Orbit(vec3(0,0,0), App->input->GetMouseXMotion(), App->input->GetMouseYMotion());
			else
				Orbit(vec3(aux->GetPosition().x, aux->GetPosition().y, aux->GetPosition().z), App->input->GetMouseXMotion(), App->input->GetMouseYMotion());

		}
			
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		//insert last gizmo position and the distance
		//distance has to change depending on the size of the imported fbx
		if(aux== NULL)
			Focus(vec3(0, 0, 0), STD_CAM_DISTANCE);
		else
			Focus(vec3(aux->GetPosition().x, aux->GetPosition().y, aux->GetPosition().z), STD_CAM_DISTANCE);

	}
	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * rot_speed;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * rot_speed;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}

	// Recalculate matrix -------------
	CalculateViewMatrix();

	App->performance.SaveRunTimeData(name); 

	return UPDATE_CONTINUE;
}
void ModuleCamera3D::Focus(const vec3& focus, const float& distance)
{
	Reference = focus;
	//TODO change it to a simple rotation
	Position = Reference + (Z * distance);
}
void ModuleCamera3D::Orbit(const vec3& orbit_center, const float& motion_x, const float& motion_y)
{
	Reference = orbit_center;

	int dx = -motion_x;
	int dy = -motion_y;

	Position -= Reference;

	if (dx != 0)
	{
		//Y rotation
		X = rotate(X, dx, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, dx, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, dx, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		//X rotation
		Y = rotate(Y, dy, X);
		Z = rotate(Z, dy, X);
	}

	Position = Reference + Z * length(Position);
}
// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::DragFloat3("Position", &Position.x, 0.1f);
		if (ImGui::Button("Original Position") == true)
		{
			Position.x = 1.0f;
			Position.y = 1.0f;
			Position.z = 5.0f;
		}

		ImGui::DragFloat("Mov Speed", &mov_speed, 0.1f, 0.1f);
		ImGui::DragFloat("Rot Speed", &rot_speed, 0.05f, 0.01f);
		ImGui::DragFloat("Zoom Speed", &mouse_wheel_speed, 0.1f, 2.0f,50.0f);

		
		if (ImGui::Button("Original Speeds") == true)
		{
			mov_speed = 0.1f;
			rot_speed = 0.25f;
			zm_speed = 0.1f;
		}
	}
}