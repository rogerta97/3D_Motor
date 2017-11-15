#pragma once 

#include <vector>
#include <map>
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
	void ClearOctreeNode(); 

	bool InsertToNode(GameObject* new_go, uint max_objects);
	void DeleteNode(); 	
	void DrawNode(); 
	void SplitNode(int max);

	template <typename TYPE>
	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& tester);

	vec GetCenter() const; 

public: 

	AABB box; 
	vector<GameObject*> objects_in_node; 
	OctreeNode* child_nodes[8];
	OctreeNode* parent_node = nullptr; 

	bool leaf = true; 
};

class Octree
{
public: 

	Octree(); 
	~Octree(); 

	void Create(AABB limits, int max_objects);
	void ClearOctree();
	void Insert(GameObject* new_go);
	void Remove(GameObject* to_delete);

	void Split(); 

	void DrawOctree();

	void SetActive(bool _active); 

	template <typename TYPE>
	void CollectIntersections(std::map<float, GameObject*>& objects, const TYPE& tester);

	// utility

	OctreeNode* GetLastLeafNode(); 
	OctreeNode* GetCurrentChildNode(GameObject* go); 
	OctreeNode* GetRootNode(); 
	uint GetNumObjects(); 

private: 

	OctreeNode* root_node = nullptr; 
	bool active = false; 
	uint num_objects_added; 
	uint max_objects; 

};

template<typename TYPE>
inline void OctreeNode::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE & tester)
{
	if (tester.Intersects(box))
	{

		float close_distance, far_distance; 
		for (std::vector<GameObject*>::iterator it = objects_in_node.begin(); it != objects_in_node.end(); ++it)
		{
			if (tester.Intersects((*it)->GetBoundingBox(), close_distance, far_distance))
				objects[close_distance] = (*it);
		}

		for (int i = 0; i < 4; ++i)
			if (child_nodes[i] != nullptr) child_nodes[i]->CollectIntersections(objects, tester);
	}
}

template<typename TYPE>
inline void Octree::CollectIntersections(std::map<float, GameObject*>& objects, const TYPE & tester)
{
	if (root_node != nullptr)
		root_node->CollectIntersections(objects, tester); 

		
}
