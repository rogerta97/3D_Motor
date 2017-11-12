#include "RayCast.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "moduleImGui.h"

RayCast::RayCast()
{
}

void RayCast::Update()
{
	if (ImGui::IsMouseClicked(0))
	{
		ImVec2 pos = ImGui::GetMousePos(); 
		
		ray = App->camera->GetEditorCam()->frustum.UnProjectLineSegment(pos.x / SCREEN_WIDTH, pos.y / SCREEN_HEIGHT);
		
	}

	DebugDraw(ray);
}

RayCast::~RayCast()
{
}
