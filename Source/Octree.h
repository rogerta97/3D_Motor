#pragma once 

#include <vector>
#include "GameObject.h"
#include "MathGeoLib\MathGeoLib.h"

using namespace std; 

class OctreeNode
{

public:

	OctreeNode(AABB box);
	~OctreeNode();

	bool IsFull(); 
	bool IsLeaf(); 

	bool InsertToNode(AABB& new_go);

	void DeleteNode(); 
	
	void DrawNode(); 
	void SplitNode();

	vec GetCenter() const; 

public: 

	AABB box; 
	vector<AABB> objects_in_node; 
	OctreeNode* child_nodes[8];
	OctreeNode* parent_node; 

	bool leaf = true; 
};

class Octree
{
public: 

	Octree(); 
	~Octree(); 

	void Create(AABB limits);
	void Clear();
	void Insert(GameObject* new_go);
	void Remove(GameObject* to_delete);

	void Split(); 

	void DrawOctree();

	void SetActive(bool _active); 

	// utility

	OctreeNode* GetLastLeafNode(); 
	OctreeNode* GetCurrentChildNode(GameObject* go); 
	OctreeNode* GetRootNode(); 
	uint GetNumObjects(); 

private: 

	OctreeNode* root_node; 
	bool active = false; 
	uint num_objects_added; 

};


