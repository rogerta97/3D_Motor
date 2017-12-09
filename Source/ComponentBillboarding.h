#ifndef _COMPONENTBILLBOARDING_H_
#define _COMPONENTBILLBOARDING_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class json_file; 
class GameObject; 
class ComponentCamera; 

class ComponentBillboarding : public Component
{
public:

	ComponentBillboarding(GameObject* parent);
	~ComponentBillboarding();

	//Parent

	bool Update();
	void OnLoad(json_file* config);
	void OnSave(json_file& config)const;
	void Delete();

	void SetReference(ComponentCamera* reference);
	ComponentCamera* GetReference();

	bool y_axis_locked;
	bool x_axis_locked;

	bool bill_active;
	bool ref_position_changed;

private:

	ComponentCamera* reference;			//Billboard will always be aligned to a camera

	float3 last_ref_pos;				//For optimization, knowing if the camera has moved and aply the billboarding calculations 

	float angle_x; 
	float angle_y; 
	float angle_z; 


};

#endif
