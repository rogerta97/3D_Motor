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

	void AttachObject(GameObject* new_reference);

	void SetShowInputWindow(bool new_set); 
	bool GetShowInputWindow() const;

private:

	GameObject* reference; 

	float3 curr_relation; 
	bool show_set_window; 

};

#endif
