#ifndef _COMPONENTCAMERA_H_
#define _COMPONENTCAMERA_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"


class ComponentCamera : public Component
{
public: 

	ComponentCamera(); 

	~ComponentCamera(); 

	bool Update(); 

	void DrawFrustum(); 

private:

	Frustum frustum; 
	bool frustum_culling = false; 

};

#endif
