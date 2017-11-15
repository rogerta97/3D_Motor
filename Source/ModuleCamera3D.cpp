#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "ModuleFBXLoader.h"
#include "ComponentDefs.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "GameObject.h"

#define STD_CAM_DISTANCE 20
ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	mov_speed = 0.1f;
	rot_speed = 0.25f;
	zm_speed = 0.1f;
	mouse_wheel_speed = 2.0f;
	near_plane = 0.1f;
	far_plane = 100.0f;
	h_field_of_view = 60.85f;
	v_field_of_view = 60.0f;
	aspect_ratio = 1.3;

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	editor_camera = new ComponentCamera(nullptr, far_plane, near_plane, h_field_of_view, h_field_of_view / v_field_of_view);
}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(editor_camera);
}

bool ModuleCamera3D::Init(json_file* config)
{
	name = "Camera";
	App->performance.InitTimer(name);
	App->renderer3D->curr_cam = editor_camera;
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
	App->renderer3D->curr_cam = nullptr;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	GameObject* aux;
	Frustum frustum = editor_camera->frustum;

	ComponentTransform* tmp_trans= nullptr; 

	if (App->scene_intro->GetGameObject(0) != nullptr)
		 aux = App->scene_intro->GetGameObject(0);
	else
		 aux = nullptr;
	App->performance.InitTimer(name); 
	//------Move
	Move();
	//------Orbit with Focus

	//if((App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_DOWN))
	//	is_rotating = true;

	//else if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_UP|| App->input->GetKey(SDL_SCANCODE_RALT) == KEY_UP)
	//	is_rotating = false;
	
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RALT) == KEY_REPEAT))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		editor_camera->Rotate(dx*rot_speed*0.01f, dy*rot_speed*0.01f);		
	}

		

	Position -= Reference;

	//------Focus 
	//(if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	//{
	//	//insert last gizmo position and the distance
	//	//distance has to change depending on the size of the imported fbx
	//	tmp_trans = (ComponentTransform*)App->scene_intro->GetGameObject(0)->GetComponent(COMPONENT_TRANSFORM);
	//
	//	if(aux== NULL)
	//		Focus(vec3(0, 0, 0), STD_CAM_DISTANCE);
	//	else
	//		Focus(vec3(tmp_trans->GetLocalPosition().x, tmp_trans->GetLocalPosition().y, tmp_trans->GetLocalPosition().z), STD_CAM_DISTANCE);
	//}

	// ------ Recalculate matrix -------------
	//CalculateViewMatrix();

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
void ModuleCamera3D::Look(const float3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	editor_camera->Look(Position);
}

void ModuleCamera3D::LookAt(const float3 &objective)
{
	float3 direction = objective - editor_camera->frustum.pos;

	float3x3 matrix = float3x3::LookAt(editor_camera->frustum.front, direction.Normalized(), editor_camera->frustum.up, float3::unitY);

	editor_camera->frustum.front = matrix.MulDir(editor_camera->frustum.front).Normalized();
	editor_camera->frustum.up = matrix.MulDir(editor_camera->frustum.up).Normalized();
}

// -----------------------------------------------------------------

// -----------------------------------------------------------------
void ModuleCamera3D::Move()
{
	vec3 move_aux(0.0f, 0.0f, 0.0f);

	float temporal_speed = mov_speed;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		temporal_speed = temporal_speed * 0.3f;

	if (App->input->IsMouseInWindow() == 0)
	{
		if (App->input->GetMouseWheel() == 1) editor_camera->MoveForward(temporal_speed);
		if (App->input->GetMouseWheel() == -1) editor_camera->MoveBackwards(temporal_speed);
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) editor_camera->MoveForward(temporal_speed);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) editor_camera->MoveBackwards(temporal_speed);

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) editor_camera->MoveLeft(temporal_speed);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) editor_camera->MoveRight(temporal_speed);

	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) editor_camera->MoveDown(temporal_speed);
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) editor_camera->MoveUp(temporal_speed);

	float3 frustum_move(move_aux.x, move_aux.y, move_aux.z);
	editor_camera->frustum.SetPos(editor_camera->frustum.pos + frustum_move);

	//CalculateViewMatrix();
}

void ModuleCamera3D::Move(const vec3 & pos)
{
	Position += pos;
	Reference += pos;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::FreeOrbit()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * rot_speed;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * rot_speed;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}

	Position = Reference + Z * length(Position);
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

float* ModuleCamera3D::GetFrustumViewMatrix()
{
	static float4x4 m;

	m = editor_camera->frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v; 
}

float * ModuleCamera3D::GetFrustumProjectionMatrix()
{
	return editor_camera->GetOpenGLProjectionMatrix(); 
}

void ModuleCamera3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::DragFloat("Mov Speed", &mov_speed, 0.1f, 0.1f);
		ImGui::DragFloat("Rot Speed", &rot_speed, 0.05f, 0.01f);
		ImGui::DragFloat("Zoom Speed", &mouse_wheel_speed, 0.1f, 2.0f,50.0f);
		
		if (ImGui::Button("Reset"))
		{
			Position.x = 1.0f;
			Position.y = 1.0f;
			Position.z = 5.0f;
			editor_camera->SetPosition({ Position.x,Position.y,Position.z });

			mov_speed = 0.1f;
			rot_speed = 0.25f;
			zm_speed = 0.1f;
		}

		ImGui::Separator(); 
		ImGui::Separator();

		if (ImGui::Checkbox("Frustum Culling", &frustum_culling)); 
		//editor_camera->frustum.SetFrame({ 0, 2, -5 }, float3::unitZ, float3::unitY);

		float aux_n_p = editor_camera->GetNearPlaneDist();
		if (ImGui::DragFloat("Near Plane", &aux_n_p, 0.1f, 0.1f, 1000.0f))
		{
			editor_camera->SetNearPlaneDist(aux_n_p);
			editor_camera->frustum.SetViewPlaneDistances(aux_n_p, editor_camera->frustum.farPlaneDistance);
		}
			

		float aux_f_p = editor_camera->GetFarPlaneDist();
		if (ImGui::DragFloat("Far Plane", &aux_f_p, 0.1f, 0.1f, 10000.0f))
		{
			editor_camera->SetFarPlaneDist(aux_f_p);
			editor_camera->frustum.SetViewPlaneDistances(editor_camera->frustum.nearPlaneDistance, aux_f_p);
		}
		

		float aux_VFOV = editor_camera->GetFOV();
		if (ImGui::SliderFloat("Field Of View", &aux_VFOV, 1.0f, 179.0f, "%.2f"))
		{
			editor_camera->SetFOV(aux_VFOV);
		}
			

		float aux_AR = editor_camera->GetAspectRatio();
		if(ImGui::DragFloat("Aspect Ratio", &aux_AR, 0.1f, 0.1f, 10000.0f))
			editor_camera->SetAspectRatio(aux_AR);


	}
}

void ModuleCamera3D::SetCamPosition(math::float3 position)
{
	editor_camera->frustum.Translate(position);
}

ComponentCamera * ModuleCamera3D::GetEditorCam() const
{
	return editor_camera;
}

bool ModuleCamera3D::IsCulling()
{
	return frustum_culling;
}
