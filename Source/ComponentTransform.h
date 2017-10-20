#ifndef _COMPONENTTRANFORM_H_
#define _COMPONENTTRANFORM_H_

#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();

	~ComponentTransform();

	bool Update();

	void SetPosition(const float3& position);
	void SetRotation(const Quat& rotation);
	void SetScale(const float3& scale);

	float3 GetLocalPosition() const;
	Quat GetLocalRotation() const;
	float3 GetLocalScale() const;

	void SetTransformMatrix();
	const float* GetGlobalTransform(); 

private:
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	float4x4 transform_matrix; 
};

#endif 


