#pragma once

#include "Component.h"
#include "Timer.h"
#include "ComponentDefs.h"



enum particle_system_state
{
	PARTICLE_STATE_PLAY,
	PARTICLE_STATE_PAUSE,
};

struct ParticleComponents
{
	ComponentMeshRenderer* particle_mesh;
	ComponentTransform* particle_transform;
	ComponentMaterial* particle_material; 
	ComponentBillboarding* particle_billboarding;

	void SetToNull()
	{
		particle_mesh = nullptr; 
		particle_transform = nullptr; 
		particle_material = nullptr; 
		particle_billboarding = nullptr; 
	}
};

class Particle
{
public:

	Particle();

	void Update();

	void Delete();

	//Drawing
	void Draw(); 
	void DrawMesh(); 

	//Setters & Getters
	ParticleComponents GetAtributes();

	void SetMaxLifetime(float new_lifetime);
	float GetMaxLifeTime();

	bool IsDead();

	~Particle(); 

public: 

	ParticleComponents atributes;

private:

private:

	Timer particle_timer; 
	float max_particle_lifetime; 
	bool kill_me;

};

class ComponentParticleEmmiter : public Component
{

public:
	ComponentParticleEmmiter(GameObject* parent); 

	void Start(); 
	void CreateRootParticle(); 

	bool Update();
	void UpdateRootParticle(); 

	~ComponentParticleEmmiter(); 

	//Spawning
	void GenerateParticles(); 
	Particle* CreateParticle(); 
	int GetParticlesNum(); 

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

	//UI data
	float max_lifetime;						//Time that particules will be rendering
	int emmision_rate;						//How many spawn in a second

private:

private: 

	//Emmiter surface 
	ComponentMeshRenderer* emit_area; 

	//General Management
	float particles_lifetime;				 //Lifetime of the particules spawned
	particle_system_state system_state;		 //Inner play & pause 
	Particle* root_particle;				 //This will be the particle that will be cloned over time
	list<Particle*> active_particles;		 //Particles that are currently beeing rendered

	//Spawn Management
	float emmision_frequency;				//Difference between spawn

	Timer spawn_timer; 

	//Debug
	bool show_emit_area;					//boolean for showing the emmiter area
	vector<uint> shapes_ids;				//This list will hold the id's of the textures that can give shape to the particles
	uint shapes_amount; 



};
