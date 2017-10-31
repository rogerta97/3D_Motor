#ifndef _COMPONENTTRANFORM_H_
#define _COMPONENTTRANFORM_H_

#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);

	~ComponentTransform();

	bool Update();

	void SetPosition(const float3& position);
	void SetRotation(const Quat& rotation);
	void SetRotation(const float3& rotation);
	void SetScale(const float3& scale);

	float3 GetLocalPosition();
	Quat GetLocalRotation();
	float3 GetLocalScale(); 

	void SetTransformMatrix();
	const float* GetGlobalTransform(); 
	int GetTransformID(); 

	void SetIdentityTransform(); 

private:
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	float4x4 transform_matrix; 

	int transform_id; 
};

#endif 


