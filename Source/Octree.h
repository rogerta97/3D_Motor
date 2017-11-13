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

	bool InsertToNode(GameObject* new_go);
	void DeleteNode(); 	
	void DrawNode(); 
	void SplitNode();

	template <typename TYPE>
	void CollectIntersections(list<GameObject*>& objects, const TYPE& tester);

	vec GetCenter() const; 

public: 

	AABB box; 
	vector<GameObject*> objects_in_node; 
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

	template <typename TYPE>
	void CollectIntersections(list<GameObject*>& objects, const TYPE& tester);

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

template<typename TYPE>
inline void OctreeNode::CollectIntersections(list<GameObject*>& objects, const TYPE & tester)
{
	if (tester.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->global_bbox))
				objects.push_back(*it);
		}

		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);
	}
}

template<typename TYPE>
inline void Octree::CollectIntersections(list<GameObject*>& objects, const TYPE & tester)
{
	if (root_node != nullptr)
		root_node->CollectIntersections(objects, tester); 
}
