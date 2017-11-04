#include "Octree.h"
#include "DebugDraw.h"
#include "ComponentMeshRenderer.h"

Octree::Octree()
{
}

Octree::~Octree()
{
}

void Octree::Create(AABB limits)
{
	root_node = new OctreeNode(); 

	root_node->box = limits; 

	root_node->child_nodes.clear();
	root_node->objects_in_node.clear();
	root_node->leaf = true; 

}

void Octree::Clear()
{

}

void Octree::Insert(GameObject * new_go)
{
	ComponentMeshRenderer* mr = (ComponentMeshRenderer*)new_go->GetComponent(COMPONENT_MESH_RENDERER); 

	OctreeNode* curr_node;

	if (root_node != nullptr)
		curr_node = root_node;

	curr_node->InsertToNode(mr->bounding_box);
}

void Octree::Remove(GameObject * to_delete)
{
}

void Octree::Split(OctreeNode* node_to_split)
{
	node_to_split->IsLeaf(); 
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

OctreeNode* Octree::GetLastLeafNode()
{
	OctreeNode* ret;

	//if (root_node != nullptr)
	//	ret = root_node; 

	//while (ret != nullptr)
	//{
	//	if (!ret->child_nodes.empty())
	//	{
	//		GetCurrentChildNode(); 
	//	}
	//}



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



OctreeNode::~OctreeNode()
{

}

bool OctreeNode::IsLeaf()
{
	return leaf;
}

bool OctreeNode::InsertToNode(AABB& new_go_AABB)
{
	bool ret = true; 

	//if () //Before starting we check if the object is already in the octree
	//{
		if (box.Contains(new_go_AABB))
		{
			if (IsLeaf())
			{
				switch (objects_in_node.size())
				{
				case 0:
					objects_in_node.push_back(new_go_AABB);
					break;

				case 1:
					objects_in_node.push_back(new_go_AABB);
					Split(); 
					break;
				}
			}
			else
			{
				for (int i = 0; i < child_nodes.size(); i++)
				{
					if (child_nodes[i]->box.Contains(new_go_AABB))
						child_nodes[i]->InsertToNode(new_go_AABB);
				}
			}
		}
		else
			ret = false;
	//}


	return ret; 
}

void OctreeNode::DeleteNode()
{
	if (!child_nodes.empty())
	{
		objects_in_node.clear();
		parent_node = nullptr; 

		delete(this);
	}
		
	else
	{
		for (int i = 0; i < child_nodes.size(); i++)
		{
			child_nodes[i]->DeleteNode(); 
		}
	}
}

void OctreeNode::Split()
{

	// First we split the node in 9 childs

	float new_size = box.HalfSize().x; 

	vec pivot_point = GetCenter(); 
	vec max_points_new_childs[8]; 

	max_points_new_childs[0] = vec(-new_size, new_size, -new_size); 
	max_points_new_childs[1] = vec(new_size, new_size, -new_size);
	max_points_new_childs[2] = vec(new_size, new_size, new_size);
	max_points_new_childs[3] = vec(-new_size, new_size, new_size);
	max_points_new_childs[4] = vec(-new_size, -new_size, -new_size);
	max_points_new_childs[5] = vec(new_size, -new_size, -new_size);
	max_points_new_childs[6] = vec(new_size, -new_size, new_size);
	max_points_new_childs[7] = vec(-new_size, -new_size, new_size);

	for (int i = 0; i < 8; i++)
	{
		OctreeNode* new_node = new OctreeNode();

		new_node->box = AABB(pivot_point, max_points_new_childs[i]);
		new_node->leaf = true;
		new_node->parent_node = this; 

		// Here we modify each parent 

		for (int j = 0; j < objects_in_node.size(); j++)
		{
			if (new_node->box.Contains(objects_in_node[j]))
			{
				new_node->objects_in_node.push_back(objects_in_node[j]);
			}
		}

		child_nodes.push_back(new_node);
		leaf = false; 

		// We check if everything is correct in case more than 2 objects in the same node

		if (new_node->objects_in_node.size() == 2)
		{
			new_node->Split(); 
		}
	}

}

vec OctreeNode::GetCenter() const
{
	return box.CenterPoint(); 
}

void OctreeNode::GetObjectsInNode(int& amount)
{
	if (leaf == false)
	{
		for (int i = 0 ; i < child_nodes.size(); i++)
		{
			child_nodes[i]->GetObjectsInNode(amount); 
		}
	}
	else
	{
		amount += objects_in_node.size(); 
	}

}

void OctreeNode::DrawNode()
{
	DebugDraw(box, Green); 

	for (int i = 0; i < objects_in_node.size(); i++)
	{
		DebugDraw(objects_in_node[i], Red);
	}

	if (!child_nodes.empty())
	{
		for (int i = 0; i < child_nodes.size(); i++)
		{
			child_nodes[i]->DrawNode();
		}
	}

}

OctreeNode::OctreeNode()
{

}
