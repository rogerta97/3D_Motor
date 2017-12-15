#ifndef _COMPONENTTRANFORM_H_
#define _COMPONENTTRANFORM_H_

#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

struct Transform
{
	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

};

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* parent);

	~ComponentTransform();

	bool Update();
	void OnLoad(json_file* config);
	void OnSave(json_file& config)const;
	void DeleteComponent();
	// Local Pos Setters & Getters

	void SetLocalPosition(const float3& position);
	void SetLocalRotation(const float3& rotation);
	void SetLocalScale(const float3& scale);

	float3 GetLocalPosition()const;
	Quat GetLocalRotation()const;
	float3 GetLocalScale()const;

	float3 GetGlobalPosition();
	Quat GetGlobalRotation();
	float3 GetGlobalScale();

	// Local Transform Setters & Getters

	void SetLocalTransformMat(float4x4 new_transform);
	float4x4 GetLocalTransform();

	void SetIdentityTransform(); 

	// Global Transform Setters & Getters 

	void SetGlobalTransform(float4x4 new_transform);
	float4x4 GetGlobalTransform();

	// Utility
	float3 LocalX();
	float3 LocalY();
	float3 LocalZ();

	void DrawAxis();
	

	bool IsModified() const; 
	void SetModified(bool value);
	bool IsPositionChanged() const;
	void SetPositionChanged(bool new_set); 
	void UpdateTransform(); 
	void Serialize(json_file* file);
	void Serialize(json_file file);


	Transform transform;
	Transform global_transform;

	float4x4 local_transform_mat;
	float4x4 global_transform_mat;

private:

	Transform prev_transform; 



	bool transform_modified = false; 
	bool position_modified = false; 
};

#endif 


