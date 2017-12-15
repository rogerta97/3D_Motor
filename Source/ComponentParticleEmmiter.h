#pragma once

#include "Component.h"
#include "Timer.h"
#include "ComponentDefs.h"
#include <map>


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

	//Setters & Getters
	ParticleComponents GetAtributes();

	//Billboarding
	void SetBillboardReference(ComponentCamera* new_reference);
	ComponentCamera* GetBillboardReference();

	//Interpolation
	///Color
	void UpdateColor();
	bool IsInterpolatingColor() const;
	void SetInterpolatingColor(bool interpolate, Color initial_color, Color final_color);

	///Scale
	void UpdateSize(); 
	void SetInterpolationSize(bool interpolate, float3 initial_scale, float3 final_scale);

	///Rotation
	void UpdateRotation();
	void SetInterpolationRotation(bool active, float initial_v, float final_v);

	///Force calculations
	void SetMovement(float3 mov); 
	void SetGravity(float3 grav);

	///Velocity
	void SetVelocity(const float& new_velocity);
	float GetVelocity() const;

	///Rotation
	void SetAngular(const float& new_velocity);
	float GetAngular() const;

	///Lifetime
	void SetMaxLifetime(const float& new_lifetime);
	float GetMaxLifeTime() const;

	///Texture
	void SetTextureByID(uint texture_ID);

	///Color
	void SetColor(Color new_color); 
	Color GetColor() const; 

	Color GetInitialColor() const; 
	Color GetFinalColor() const; 

	void SetInitialColor(Color color);
	void SetFinalColor(Color color);

	void SetDistanceToCamera(float new_dist); 
	float GetDistanceToCamera(); 

	bool IsDead();

	~Particle(); 

public: 

	ParticleComponents components;

private:

private:

	Timer particle_timer; 
	float max_particle_lifetime; 

	//Color
	Color particle_color;

	//Interpolations
	bool interpolate_colors;
	Color initial_particle_color;
	Color final_particle_color;

	bool interpolate_size; 
	float3 initial_particle_size;
	float3 final_particle_size;

	bool interpolate_rotation;
	Timer twister; 
	float initial_particle_angular_v;
	float final_particle_angular_v;
	float curr_rot; 

	int color_difference[4]; 

	Timer interpolation_timer; 

	//Managing movement
	float particle_velocity;
	float3 particle_gravity;
	float particle_angular_v;

	float3 movement;						//This vector will be added to the position every frame

	bool kill_me;
	int particle_texture_id; 
	float distance_to_camera; 

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
	void DrawEmisionArea(bool show); 

	int GetTextureID(int pos);
	int GetTextureIDAmount();

	void SetCurrentTextureID(uint texture_id); 
	uint GetCurrentTextureID() const; 

	Particle* GetRootParticle() const;

public:

	//UI data ------

	//Emiter

	//Shape
	uint curr_texture_id;					//Hold the texture that will be drawn into the particles that we are cloning

	//Color
	Color color;
	float angular_v; 

	//Interpolations
	int initial_color[4];
	int final_color[4];

	float3 initial_scale; 
	float3 final_scale; 

	float initial_angular_v;
	float final_angular_v;

	bool apply_color_interpolation;			//If true, the particles instanciated will be given 2 colors and they will interpolate between them
	bool apply_size_interpolation; 
	bool apply_rotation_interpolation;

	//Motion
	bool show_billboarding;
	bool lock_billboarding_y;
	bool lock_billboarding_x;

	float max_lifetime;						//Time that particules will be rendering
	int emmision_rate;						//How many spawn in a second
	float velocity;							//Velocity at what the particles are emmited

	float3 gravity;							//Gravity
	float emision_angle;					//This is the max angle thet will be given to the particle

	// --------

	//Emmiter surface 
	ComponentMeshRenderer* emit_area;

private:

private: 

	//General Management
	float particles_lifetime;				 //Lifetime of the particules spawned
	particle_system_state system_state;		 //Inner play & pause 
	Particle* root_particle;				 //This will be the particle that will be cloned over time
	list<Particle*> active_particles;		 //Particles that are currently beeing rendered
	multimap<float, Particle*> particles_sorted;
	Timer reorder_time;

	//Spawn Management
	float emmision_frequency;				//Difference between spawn
	Timer spawn_timer; 

	//Debug
	bool show_emit_area;					//boolean for showing the emmiter area
	vector<uint> shapes_ids;				//This list will hold the id's of the textures that can give shape to the particles
	uint shapes_amount; 



};
