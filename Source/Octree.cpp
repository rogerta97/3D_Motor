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

}

void Octree::Clear()
{
}

void Octree::Insert(GameObject * new_go, OctreeNode& curr_node)
{
	ComponentMeshRenderer* mr = (ComponentMeshRenderer*)new_go->GetComponent(COMPONENT_MESH_RENDERER); 

	if (mr != nullptr && curr_node.box.Contains(mr->bounding_box))
	{
		if (curr_node.IsLeaf())
		{
			Split(); 
		}
		else
		{
			for (int i = 0; i < 8; i++)
			{				
				Insert(new_go, *curr_node.child_nodes[i]);
			}
		}
	}
	else
		LOG("Object need a mesh for being inserted to Octree"); 
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
	return child_nodes.empty();
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
