#include "RayCast.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentMeshRenderer.h"
#include "moduleImGui.h"

RayCast::RayCast()
{
}

void RayCast::Update()
{
	if (ImGui::IsMouseClicked(0))
	{
		int width = App->window->width; 
		int height = App->window->height;

		int mouse_x = App->input->GetMouseX();
		int mouse_y = App->input->GetMouseY();

		float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
		float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;

		ray = App->camera->GetEditorCam()->frustum.UnProjectLineSegment(normalized_x, normalized_y);

		if (GetHit() != nullptr)
		{
			GameObject* c = GetHit(); 
			App->scene_intro->SetCurrentGO(GetHit()->GetID()); 
			LOG("Object %s was hitted", c->GetName()); 
		}
	}

	DrawRay(); 
	
}

GameObject * RayCast::GetHit()
{
	GameObject* ret; 
	// First we get the list of AABB's the ray is coliding with ordered by distance

	GetObjectsByDistance(candidate_list); 

	// We check collisions for every triangle of the mesh of selected objects

	ret = RayTest(); 
	
	candidate_list.clear(); 

	return ret;
}

void RayCast::GetObjectsByDistance(vector<GameObject*>& objects)
{
	// This has to be improved with octree algorithm

	for (int i = 0; i < App->scene_intro->GetList().size(); i++)
	{
		GameObject* curr_candidate = App->scene_intro->GetGameObject(i);

		if (ray.Intersects(curr_candidate->GetBoundingBox()))
		{
			LOG("Candidate Hit");
			objects.push_back(curr_candidate); 
		}
	}
}

GameObject * RayCast::RayTest()
{
	int best_distance;
	GameObject* ret = nullptr; 

	for (int i = 0; i < candidate_list.size(); i++)
	{
		ComponentMeshRenderer* tmp_mr = (ComponentMeshRenderer*)candidate_list[i]->GetComponent(COMPONENT_MESH_RENDERER); 

		for (int j = 0; j < tmp_mr->GetNumTriangles();)
		{
			float3 tri_point_1 = tmp_mr->vertices[tmp_mr->indices[j++]]; 
			float3 tri_point_2 = tmp_mr->vertices[tmp_mr->indices[j++]];
			float3 tri_point_3 = tmp_mr->vertices[tmp_mr->indices[j++]];

			Triangle tri(tri_point_1, tri_point_2, tri_point_3);

			float distance;
			float3 hit_point;

			if (ray.Intersects(tri, &distance, &hit_point))
			{
				if (best_distance > distance)
				{
					best_distance = distance;
					ret = candidate_list[i]; 
				}					
			}
		}
	}

	return ret; 
}

RayCast::~RayCast()
{
}

void RayCast::DrawRay()
{
	DebugDraw(ray, Red);
}
