#include "RayCast.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentMeshRenderer.h"
#include "ComponentTransform.h"
#include "ComponentBillboarding.h"
#include "Functions.h"
#include "Octree.h"
#include "moduleImGui.h"

RayCast::RayCast()
{
}

void RayCast::Update()
{
	if (ImGui::IsMouseClicked(0) && App->input->IsMouseInWindow() == false && App->camera->is_rotating == false)
	{
		int width = App->window->width; 
		int height = App->window->height;

		int mouse_x = App->input->GetMouseX();
		int mouse_y = App->input->GetMouseY();

		float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
		float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;

		ray = App->camera->GetEditorCam()->frustum.UnProjectLineSegment(normalized_x, normalized_y);

		if (App->scene_intro->GetGameObjectsNum() > 0)
		{
			GameObject* hited_GO = GetHit();

			if (hited_GO != nullptr)
			{
				GameObject* curr_obj = App->scene_intro->GetCurrentGO();

				if (curr_obj != nullptr && curr_obj->IsLooking4Parent() == true)
				{
					curr_obj->SetParent(hited_GO);
					curr_obj->SetLooking4Parent(false);
					LOG("%s is child of %s", curr_obj->GetName(), hited_GO->GetName());
				}
				else if (curr_obj != nullptr && curr_obj->IsLooking4Reference() == true)
				{
					ComponentBillboarding* bill = (ComponentBillboarding*)curr_obj->GetComponent(COMPONENT_BILLBOARDING); 
					bill->SetReference(hited_GO);
					curr_obj->SetLooking4Reference(false);
				}
				else 
				{
					App->scene_intro->SetCurrentGO(hited_GO->GetID());
					hited_GO->ShowBB(true);
				}
					

				LOG("Object %s was hitted and returned succesfully", hited_GO->GetName());
			}
		}
	}	

	DebugDraw(ray, Green); 
}

GameObject * RayCast::GetHit()
{
	GameObject* ret;

	map<float, GameObject*> map_candidates; 
	vector<GameObject*> candidate_list; 
	// First we get the list of AABB's the ray is coliding with ordered by distance

	if (App->scene_intro->octree != nullptr && App->scene_intro->octree->GetNumObjects() > 0)
	{
		App->scene_intro->octree->CollectIntersections(map_candidates, ray);
		candidate_list = MapToVector(map_candidates);		
	}

	if(candidate_list.empty())
		GetObjectsByDistance(candidate_list);

	//// If this is true it means that there can be objects that the ray can be coliding with and they are not tested
	//// So, collisions with octree just optimize the collisions for the objects that are inside the octree:) 

	//if (App->scene_intro->GetGameObjectsNum() > App->scene_intro->octree->GetNumObjects())
	//{
	//	G
	//}

	// We check collisions for every triangle of the mesh of selected objects
	ret = RayTest(candidate_list); 
	
	candidate_list.clear(); 

	return ret;
}

void RayCast::GetObjectsByDistance(vector<GameObject*>& objects)
{
	// This has to be improved with octree algorithm

	vector<float> hit_distance_list; 

	for (int i = 0; i < App->scene_intro->GetGameObjectsNum(); i++)
	{
		GameObject* curr_candidate = App->scene_intro->GetGameObject(i);

		float hit_distance; 
		float out_distance;

		if (ray.Intersects(curr_candidate->GetBoundingBox(), hit_distance, out_distance))
		{
			LOG("Bounding Box %d Hit", i);

			objects.push_back(curr_candidate); 
			hit_distance_list.push_back(hit_distance); 
		}
	}

	// This is for ordering the collisions

	bool changed = true; 

	while (changed && !objects.empty())
	{
		changed = false; 

		for (int i = 0; i < objects.size() - 1; i++)
		{
			if (hit_distance_list[i] > hit_distance_list[i + 1])
			{
				swap(hit_distance_list[i], hit_distance_list[i + 1]);
				swap(objects[i], objects[i + 1]);
				changed = true;
			}
				
		}
	}

}

GameObject * RayCast::RayTest(vector<GameObject*> candidate_list)
{
	float best_distance = -1;
	GameObject* ret = nullptr; 

	for (int i = 0; i < candidate_list.size(); i++)
	{
		ComponentMeshRenderer* tmp_mr = (ComponentMeshRenderer*)candidate_list[i]->GetComponent(COMPONENT_MESH_RENDERER); //For getting triangles points
		ComponentTransform* tmp_trans = (ComponentTransform*)candidate_list[i]->GetComponent(COMPONENT_TRANSFORM); //For gettin triangle points transformation

		ray.Transform(tmp_trans->GetGlobalTransform().Inverted());

		int tri_count = 0;
		for (int j = 0; j < tmp_mr->GetNumTriangles();j++)
		{
			float3 tri_point_1 = tmp_mr->vertices[tmp_mr->indices[tri_count]];
			tri_count++;

			float3 tri_point_2 = tmp_mr->vertices[tmp_mr->indices[tri_count]];
			tri_count++;

			float3 tri_point_3 = tmp_mr->vertices[tmp_mr->indices[tri_count]];
			tri_count++;

			Triangle tri(tri_point_1, tri_point_2, tri_point_3);

			float distance;
			float3 hit_point;

			if (ray.Intersects(tri, &distance, &hit_point))
			{
				if (best_distance > distance || best_distance == -1)
				{
					best_distance = distance;
					ret = candidate_list[i]; 
					break; 
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
