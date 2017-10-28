#include "Octree.h"
#include "DebugDraw.h"

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

}

void Octree::Clear()
{
}

void Octree::Insert(GameObject * new_go)
{

}

void Octree::Remove(GameObject * to_delete)
{
}

void Octree::Split()
{
}

void Octree::DrawOctree()
{
	if (root_node != nullptr)
	{
		root_node->DrawNode();
	}
}

OctreeNode::~OctreeNode()
{

}

bool OctreeNode::IsLeaf()
{
	return false;
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
