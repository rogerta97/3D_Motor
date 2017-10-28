#pragma once 

#include <vector>
#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

using namespace std; 

class OctreeNode
{

public:

	OctreeNode();
	~OctreeNode();

	bool IsLeaf(); 

	void DrawNode(); 

public: 

	AABB box; 
	vector<AABB*> objects_in_node; 
	vector<OctreeNode*> child_nodes;
	OctreeNode* parent_node; 
};

class Octree
{
public: 

	Octree(); 
	~Octree(); 

	void Create(AABB limits);
	void Clear();
	void Insert(GameObject* new_go, OctreeNode& curr_node);
	void Remove(GameObject* to_delete);

	void Split(); 

	void DrawOctree();

private: 

	OctreeNode* root_node; 

};


