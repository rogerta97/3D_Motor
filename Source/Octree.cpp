#include "Octree.h"
#include "DebugDraw.h"
#include "ComponentMeshRenderer.h"
#include "ModuleSceneIntro.h"
#include "Application.h"

Octree::Octree()
{
}

Octree::~Octree()
{
}

void Octree::Create(AABB limits, int _max_objects, bool _adaptative)
{
	if (root_node != nullptr)
		ClearOctree(); 

	root_node = new OctreeNode(limits); 
	num_objects_added = 0; 
	max_objects = _max_objects; 
	adaptative = _adaptative;

	LOG("New Octree has been created"); 
}

void Octree::ClearOctree()
{
	if (root_node == nullptr)
	{
		LOG("Octree deleted");
	}		
	else
	{
		root_node->ClearOctreeNode();
		root_node = nullptr; 
	}
	
}

bool Octree::Insert(GameObject * new_go)
{
	bool ret = false; 

	ComponentMeshRenderer* mr = (ComponentMeshRenderer*)new_go->GetComponent(COMPONENT_MESH_RENDERER); 

	OctreeNode* curr_node;

	if (root_node != nullptr)
		curr_node = root_node;

	if (curr_node->box.Contains(new_go->GetBoundingBox()))
	{
		if (curr_node->InsertToNode(new_go, max_objects))
		{
			num_objects_added++;
			LOG("GameObject '%s' added to Octree succesfully", new_go->GetName());
			ret = true;
		}
	}
	else if (adaptative)
	{
		AdaptQuadtree(); 
	}

	return ret; 
}

void Octree::Remove(GameObject * to_delete)
{
}

bool Octree::IsAdaptative()
{
	return adaptative;
}

void Octree::SetAdaptative(bool new_adaptative)
{
	adaptative = new_adaptative; 
}


void Octree::DrawOctree()
{
	if (!active)
		return; 

	if (root_node != nullptr)
	{
		root_node->DrawNode();
	}
}

void Octree::SetActive(bool _active)
{
	active = _active; 
}

AABB Octree::AdaptQuadtree()
{
	AABB ret({ 0.0f,0.0f ,0.0f }, { 0.0f ,0.0f ,0.0f }); 

	// Destroy old octree

	ClearOctree(); 
	App->scene_intro->octree = nullptr; 
	delete(App->scene_intro->octree); 

	// Create the updated one

	Octree* new_octree; 

	GameObject* far_go = App->scene_intro->GetFarestObjectFrom({ 0,0,0 }); 
	float half_edge = far_go->DistanceTo({ 0,0,0 }); 

	int increment = far_go->GetBoundingBox().HalfSize().Length();

	AABB limits({-half_edge - increment,-half_edge - increment,-half_edge - increment,}, { half_edge + increment, half_edge + increment, half_edge + increment });

	new_octree = new Octree();
	new_octree->Create(limits, max_objects, true);

	for (int i = 0; i < App->scene_intro->GetStaticGOList().size();i++)
	{
		new_octree->Insert(App->scene_intro->GetStaticGO(i));
	}

	App->scene_intro->octree = new_octree; 

	return ret;
}

OctreeNode* Octree::GetLastLeafNode()
{
	OctreeNode* ret;

	
	return ret; 

}

OctreeNode * Octree::GetCurrentChildNode(GameObject* go)
{

	return nullptr;
}

OctreeNode * Octree::GetRootNode()
{
	return root_node;
}

uint Octree::GetNumObjects()
{
	return num_objects_added;
}

OctreeNode::~OctreeNode()
{

}

bool OctreeNode::IsLeaf()
{

	if (child_nodes[0] == nullptr)
		return true;
	else
		return false; 

}

void OctreeNode::ClearOctreeNode()
{

	if (child_nodes[0] == nullptr)
	{
		objects_in_node.clear();

		if (parent_node != nullptr)
		{
			delete(parent_node);
			parent_node = nullptr; 
		}
			

		delete(this);

	}
	else
	{
		for (int i = 0; 8 < 8; i++)
		{
			child_nodes[i]->ClearOctreeNode(); 
		}
	}
}

bool OctreeNode::IsFull()
{

	if (objects_in_node.size() == 3)
		return true;
	else
		return false; 
}

bool OctreeNode::InsertToNode(GameObject* new_go, uint max_objects)
{
	bool ret = false; 

	if (box.Contains(new_go->GetBoundingBox()))
	{
		if (IsLeaf())
		{
			ret = true;
			objects_in_node.push_back(new_go);

			if (objects_in_node.size() == max_objects)
			{
				SplitNode(max_objects);
			}
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{
				if (child_nodes[i]->box.Contains(new_go->GetBoundingBox()))
				{
					child_nodes[i]->InsertToNode(new_go, max_objects);
					ret = true;
				}

			}
		}
	}
	
			
		

	return ret; 
}

void OctreeNode::DeleteNode()
{
	if (child_nodes[0] != nullptr)
	{
		objects_in_node.clear();
		parent_node = nullptr; 

		delete(this);
	}
		
	else
	{
		for (int i = 0; i < 8; i++)
		{
			child_nodes[i]->DeleteNode(); 
		}
	}
}

void Octree::Split()
{

	if (root_node->IsLeaf())
		root_node->SplitNode(max_objects); 
	else
		for (int i = 0; i < 8; i++)
		{
			root_node->child_nodes[i]->SplitNode(max_objects);
		}

}

vec OctreeNode::GetCenter() const
{
	return box.CenterPoint(); 
}

void OctreeNode::DrawNode()
{
	DebugDraw(box, Green); 

	for (int i = 0; i < objects_in_node.size(); i++)
	{
		DebugDraw(objects_in_node[i]->GetBoundingBox(), Red);
	}

	if (child_nodes[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			child_nodes[i]->DrawNode();
		}
	}

}

void OctreeNode::SplitNode(int max_object)
{

	if (!IsLeaf())
	{
		//If the node is not the extrem we must look to the child nodes

		for (int i = 0; i < 8; i++)
		{
			child_nodes[i]->SplitNode(max_object);
		}
	}
	else
	{

		// We create each bounding box for the child node

		float3 new_size = box.HalfSize();
		float3 center = GetCenter();
		int children_count = 0;

		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					float3 min_point(box.minPoint.x + z * new_size.x, box.minPoint.y + y * new_size.y, box.minPoint.z + x * new_size.z);
					float3 max_point(min_point.x + new_size.x, min_point.y + new_size.y, min_point.z + new_size.z);

					AABB new_box(min_point, max_point);

					child_nodes[children_count] = new OctreeNode(new_box);
					child_nodes[children_count]->parent_node = this;
					children_count++;

					LOG("Created node with size %f", new_size.x);

				}
			}
		}

		//And we change empty the list of object that had the parent, now childs must have it 

			for (int j = 0; j < 8; j++)
			{
				for (int i = 0; i < objects_in_node.size(); i++)
				{
					if (child_nodes[j]->box.Contains(objects_in_node[i]->GetBoundingBox()) || child_nodes[j]->box.Intersects(objects_in_node[i]->GetBoundingBox()))
					{
						child_nodes[j]->objects_in_node.push_back(objects_in_node[i]);
					}

					// This is in case that, after spliting, the node still need to be split
					if (child_nodes[j]->IsFull())
						child_nodes[j]->SplitNode(max_object); 
				}
			}

			//Here we delete the parent go 
			for (int i = 0; i < objects_in_node.size(); i++)
			{
				objects_in_node.erase(objects_in_node.begin() + i);
				i--;
			}
		

	}
}

OctreeNode::OctreeNode(AABB limits)
{

	for (int i = 0; i < 8; i++)
	{
		child_nodes[i] = nullptr;
	}

	box = limits;
	objects_in_node.clear();
	leaf = true;
}

