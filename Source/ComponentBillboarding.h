#ifndef _COMPONENTBILLBOARDING_H_
#define _COMPONENTBILLBOARDING_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class json_file; 
class GameObject; 
class ComponentCamera; 
class Particle; 

class ComponentBillboarding : public Component
{
public:

	ComponentBillboarding(GameObject* parent, Particle* particle_parent = nullptr);
	~ComponentBillboarding();

	//Parent

	bool Update();
	void OnLoad(json_file* config);
	void OnSave(json_file& config)const;
	void Delete();

	void SetReference(ComponentCamera* reference);
	ComponentCamera* GetReference();

	void LockY(); 
	void LockX(); 

	void UnLockY();
	void UnLockX();

	bool y_axis_locked;
	bool x_axis_locked;

	bool bill_active;
	bool ref_position_changed;


private:

	ComponentCamera* reference;			//Billboard will always be aligned to a camera
	Particle* particle_parent;			//As particles are not GO, we need the parent particle in order to modify the proper transform

	float3 last_ref_pos;				//For optimization, knowing if the camera has moved and aply the billboarding calculations 
	
	float angle_x; 
	float angle_y;  


};

#endif
