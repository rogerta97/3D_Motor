#pragma once

#include "Component.h"
#include "Timer.h"
#include "ComponentDefs.h"

enum particle_material_shape
{
	MAT_SHAPE_SQUARE,
	MAT_SHAPE_NONE,
};

enum particle_system_state
{
	PARTICLE_STATE_PLAY,
	PARTICLE_STATE_PAUSE,
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

	void GenerateParticle();

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

	void Start(); 

	bool Update();

	~ComponentParticleEmmiter(); 

	//Getters & Setters
	int GetEmmisionRate() const; 
	void SetEmmisionRate(float new_emision_rate); 
	void SetFrequencyFromRate(int rate); 

	bool ShowEmmisionArea() const; 
	void SetShowEmmisionArea(bool new_show); 

	float GetLifetime() const;
	void SetLifeTime(float new_show);

	particle_system_state GetSystemState() const;
	void SetSystemState(particle_system_state new_state); 

	//Draws & images
	void DrawEmisionArea(); 
	int GetTextureID(int pos);
	int GetTextureIDAmount();

public:

private:

private: 

	//Emmiter surface 
	ComponentMeshRenderer* emit_area; 

	//General Management
	bool in_curse;
	float particles_lifetime; 
	particle_system_state system_state;  //Inner play & pause 

	//Color Management

	//Spawn Management
	float emmision_frequency;		//Difference between spawn
	int emmision_rate;				//How many spawn in a second

	//Debug
	bool show_emit_area; 

	vector<uint> shapes_ids;			//This list will hold the id's of the textures that can give shape to the particles
	uint shapes_amount; 

};
