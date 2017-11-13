#include "RayCast.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "moduleImGui.h"

RayCast::RayCast()
{
}

void RayCast::Update()
{
	if (ImGui::IsMouseClicked(0) && App->input->IsMouseInWindow() == false)
	{
		int width = App->window->width; 
		int height = App->window->height;

		int mouse_x = App->input->GetMouseX();
		int mouse_y = App->input->GetMouseY();

		float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
		float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;

		ray = App->camera->GetEditorCam()->frustum.UnProjectLineSegment(normalized_x, normalized_y);

		GameObject* c = GetHit();

		if (c != nullptr)
		{
			LOG("Object %s was hitted and returned succesfully", c->GetName()); 
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
			LOG("Bounding Box %d Hit", i);
			objects.push_back(curr_candidate); 
		}
	}
}

GameObject * RayCast::RayTest()
{
	float best_distance = -1;
	GameObject* ret = nullptr; 

	for (int i = 0; i < candidate_list.size(); i++)
	{
		ComponentMeshRenderer* tmp_mr = (ComponentMeshRenderer*)candidate_list[i]->GetComponent(COMPONENT_MESH_RENDERER); //For getting triangles points
		ComponentTransform* tmp_trans = (ComponentTransform*)candidate_list[i]->GetComponent(COMPONENT_TRANSFORM); //For gettin triangle points transformation

		for (int j = 0; j < tmp_mr->GetNumTriangles();)
		{
			float3 tri_point_1 = tmp_mr->vertices[tmp_mr->indices[j]]; 
			j++;

			float3 tri_point_2 = tmp_mr->vertices[tmp_mr->indices[j]];
			j++; 

			float3 tri_point_3 = tmp_mr->vertices[tmp_mr->indices[j]];
			j++;

			Triangle tri(tri_point_1, tri_point_2, tri_point_3);

			tri.Transform(tmp_trans->GetGlobalTransform());

			float distance;
			float3 hit_point;

			if (ray.Intersects(tri, &distance, &hit_point))
			{
				if (best_distance > distance || best_distance == -1)
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
