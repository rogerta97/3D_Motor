#ifndef _COMPONENTTRANFORM_H_
#define _COMPONENTTRANFORM_H_

#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

struct Transform
{

	float3 X_axis = {1,0,0};
	float3 Y_axis = {0,1,0};
	float3 Z_axis = {0,0,1};

	float3 position = float3::zero;
	Quat rotation = Quat::identity;
	float3 scale = float3::one;

	void DrawAxis(); 
	void UpdateAxis(); 
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

	bool IsModified() const; 
	void SetModified(bool value);

	bool IsPositionChanged() const;
	void SetPositionChanged(bool new_set); 

	void UpdateTransform(); 

	void Serialize(json_file* file);
private:

	Transform prev_transform; 

	Transform transform; 
	Transform global_transform; 

	float4x4 local_transform_mat; 
	float4x4 global_transform_mat;

	bool transform_modified = false; 
	bool position_modified = false; 
};

#endif 


