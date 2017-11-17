#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include <vector>
#include <map>
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
	GameObject* RayTest(vector<GameObject*> candidate_list);

	~RayCast(); 

private:

	void DrawRay(); 

private:

	LineSegment ray;

};

#endif
