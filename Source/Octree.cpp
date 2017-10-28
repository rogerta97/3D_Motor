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

void Octree::DrawOctree()
{
	
	DebugDraw(root_node->box); 

}

OctreeNode::~OctreeNode()
{

}

OctreeNode::OctreeNode()
{

}
