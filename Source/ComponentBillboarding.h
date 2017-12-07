#ifndef _COMPONENTBILLBOARDING_H_
#define _COMPONENTBILLBOARDING_H_

#include "Component.h"
#include "MathGeoLib\MathGeoLib.h"

class json_file; 
class GameObject; 

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

	void SetShowInputWindow(bool new_set); 
	bool GetShowInputWindow() const;

	void SetReference(GameObject* reference); 
	GameObject* GetReference();

	bool y_axis_locked;
	bool x_axis_locked;

private:

	GameObject* reference; 

	float3 curr_relation; 

	bool show_set_window; 


};

#endif
