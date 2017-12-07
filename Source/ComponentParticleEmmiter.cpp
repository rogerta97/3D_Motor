#include "ComponentParticleEmmiter.h"

Particle::Particle()
{
	particle_timer.Start(); 
	material_shape = MAT_SHAPE_SQUARE; 
}

void Particle::Create()
{
}

void Particle::Update()
{
}

Particle::~Particle()
{
}

ComponentParticleEmmiter::ComponentParticleEmmiter(GameObject* parent)
{
	this->parent = parent;
	active = true;

	type = COMPONENT_PARTICLE_EMMITER;

	emmision_frequency = 0.5f; 
	emmision_rate = 1; 

	particles_lifetime = 0.0f; 

}

void ComponentParticleEmmiter::CreateSystem(float max_lifetime)
{



}

bool ComponentParticleEmmiter::Update()
{

	if (show_emit_area)
	{

	}

	return false;
}


ComponentParticleEmmiter::~ComponentParticleEmmiter()
{
}

int ComponentParticleEmmiter::GetEmmisionRate() const
{
	return emmision_rate; 
}

void ComponentParticleEmmiter::SetEmmisionRate(float new_emision_rate)
{
	emmision_rate = new_emision_rate; 
	SetFrequencyFromRate(emmision_rate);
}

void ComponentParticleEmmiter::SetFrequencyFromRate(int rate)
{
	if(rate != 0)
		emmision_frequency = 1 / rate;
}

bool ComponentParticleEmmiter::ShowEmmisionArea() const
{
	return show_emit_area;
}

void ComponentParticleEmmiter::SetShowEmmisionArea(bool show)
{
	show_emit_area = show; 
}

float ComponentParticleEmmiter::GetLifetime() const
{
	return particles_lifetime;
}

void ComponentParticleEmmiter::SetLifeTime(float new_life)
{
	particles_lifetime = new_life; 
}
