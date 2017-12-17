#pragma once

#include "Color.h"
#include <list>

using namespace std; 

class Particle; 
class ComponentParticleEmmiter; 

class PalmTree
{
public: 

	void PushParticles(); 

private:

	int amount; 
	ComponentParticleEmmiter* palm_particles;
};

class Torpedo
{
public: 

	void MoveTorpedo(); 
	bool IsInTop();

	ComponentParticleEmmiter* torpedo_emiter;
	Particle* torpedo_particle;					//To easily handle the particle
	
};

class Firework
{
public:

	void ShootFireWork(); 

	void UpdateFireWork(); 

	Torpedo SpawnTorpedo(int v, Color in_c, Color fin_c);
	PalmTree SpawnPalmTree();

private: 

	Torpedo torpedo;
	PalmTree palm_tree; 

	bool is_torpedo_death = false; 

};