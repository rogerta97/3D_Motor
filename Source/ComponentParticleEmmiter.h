#pragma once

#include "Component.h"
#include "Timer.h"
#include "ComponentDefs.h"

enum particle_material_shape
{
	MAT_SHAPE_SQUARE,
	MAT_SHAPE_NONE,
};

struct ParticleComponents
{
	ComponentMeshRenderer* particle_mesh;
	ComponentTransform* particle_pos;
	ComponentMaterial* particle_material; 
	ComponentBillboarding* particle_billboarding;
};

class Particle
{
public:

	Particle();

	void Create(); 
	void Update(); 

	~Particle(); 

public: 

private:

private:

	Timer particle_timer; 
	particle_material_shape material_shape; 

	ParticleComponents atributes;
};

class ComponentParticleEmmiter : public Component
{

public:
	ComponentParticleEmmiter(GameObject* parent); 

	void CreateSystem(float max_lifetime); 

	bool Update();

	~ComponentParticleEmmiter(); 

	int GetEmmisionRate() const; 
	void SetEmmisionRate(float new_emision_rate); 
	void SetFrequencyFromRate(int rate); 

	bool ShowEmmisionArea() const; 
	void SetShowEmmisionArea(bool new_show); 

	float GetLifetime() const;
	void SetLifeTime(float new_show);

public:

private:

private: 

	//Emmiter surface 
	ComponentMeshRenderer* emit_area; 

	//General Management
	bool in_curse;
	float particles_lifetime; 

	//Color Management

	//Spawn Management
	float emmision_frequency;	//Difference between spawn
	int emmision_rate;			//How many spawn in a second

	//Debug
	bool show_emit_area; 


};
