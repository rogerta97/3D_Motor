#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include <vector>
#include "MathGeoLib\MathGeoLib.h"
#include "imgui.h"

using namespace std; 
class ComponentCamera; 
class GameObject; 

class RayCast
{
public:

	RayCast(); 

	void Update(); 
	GameObject* GetHit(); 
	void GetObjectsByDistance(vector<GameObject*>& objects);
	GameObject* RayTest();

	~RayCast(); 

private:

	void DrawRay(); 

private:

	LineSegment ray;
	vector<GameObject*> candidate_list;

};

#endif
