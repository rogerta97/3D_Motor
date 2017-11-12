#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include <list>
#include "MathGeoLib\MathGeoLib.h"
#include "imgui.h"

using namespace std; 

class RayCast
{
public:

	RayCast(); 

	void Update(); 

	~RayCast(); 

private: 

	LineSegment ray; 

	list<AABB*> aabb_list; 
};

#endif
