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

	// Local Pos Setters & Getters

	void SetLocalPosition(const float3& position);
	void SetLocalRotation(const float3& rotation);
	void SetLocalScale(const float3& scale);

	float3 GetLocalPosition()const;
	Quat GetLocalRotation()const;
	float3 GetLocalScale()const;

	float3 GetGlobalPosition()const;
	Quat GetGlobalRotation()const;
	float3 GetGlobalScale()const;

	// Local Transform Setters & Getters

	void SetLocalTransform(float4x4 new_transform);
	float4x4 GetLocalTransform();

	void SetIdentityTransform(); 

	// Global Transform Setters & Getters 

	void SetGlobalTransform(float4x4 new_transform);
	float4x4 GetGlobalTransform();

	// Utility

	bool IsModified() const; 
	void SetModified(bool value);
	void UpdateTransform(); 

private:

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	float3 global_position = float3::zero;
	Quat global_rotation = Quat::identity;
	float3 global_scale = float3::one;

	float4x4 local_transform; 
	float4x4 global_transform;

	bool transform_modified = false; 
};

#endif 


