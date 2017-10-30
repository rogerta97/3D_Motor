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

	while (curr_node != nullptr)
	{
		if (curr_node->objects_in_node.size() == 2)
		{

		}

		if (curr_node->box.Contains(mr->bounding_box))
		{
			if (curr_node->objects_in_node.size() < 2)
			{
				curr_node->objects_in_node.push_back(&mr->bounding_box); 
			}
			else
			{
				Split(curr_node); 
			}
		}
	}
	

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
	if (root_node != nullptr)
	{
		root_node->DrawNode();
	}
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

OctreeNode::~OctreeNode()
{

}

bool OctreeNode::IsLeaf()
{
	return leaf;
}

void OctreeNode::DrawNode()
{
	DebugDraw(box, White); 

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