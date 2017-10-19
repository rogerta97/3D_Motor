#ifndef _COMPONENTTRANFORM_H_
#define _COMPONENTTRANFORM_H_

#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	void SetPosition(const float3& position);
	void SetRotation(const Quat& rotation);
	void SetScale(const float3& scale);

	float3 GetPosition() const;
	Quat GetRotation() const;
	float3 GetScale() const;

private:
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;
};

#endif 


