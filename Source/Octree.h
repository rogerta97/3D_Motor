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

	bool InsertToNode(AABB& new_go);
	void Split(); 

	void DrawNode(); 

public: 

	AABB box; 
	vector<AABB*> objects_in_node; 
	vector<OctreeNode*> child_nodes;
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

	void Split(OctreeNode* node_to_split); 

	void DrawOctree();

	void SetActive(bool _active); 

	// utility

	OctreeNode* GetLastLeafNode(); 
	OctreeNode* GetCurrentChildNode(GameObject* go); 

private: 

	OctreeNode* root_node; 
	bool active = false; 

};


