#include "Firework.h"
#include "ComponentParticleEmmiter.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"


void Torpedo::MoveTorpedo()
{
	torpedo_emiter->Update();
}

bool Torpedo::IsInTop()
{
	if (torpedo_particle->GetMovement().y == 0)
		return true;

	else
		return false; 
}


void PalmTree::PushParticles()
{
}

void Firework::ShootFireWork()
{
	//Get the values

	float velocity = 2.0f; 
	Color initial_col = { 255,255,255,0 }; 
	Color final_col = { 255,255,255 }; 

	// ------
	torpedo = SpawnTorpedo(velocity, initial_col, final_col);
}

void Firework::UpdateFireWork()
{
	
	if (is_torpedo_death)
	{

	}
	else
	{
		torpedo.MoveTorpedo();

		if (torpedo.IsInTop())
			is_torpedo_death = true; 
	}


}

Torpedo Firework::SpawnTorpedo(int v, Color in_c = { 255,255,255,0 }, Color fin_c = { 255,255,255,0 })
{
	Torpedo new_torpedo; 

	//Set the random values 
	//new_torpedo.torpedo_emiter = new ComponentParticleEmmiter();

	new_torpedo.torpedo_emiter->billboarding = true; 
	new_torpedo.torpedo_emiter->velocity = v; 

	new_torpedo.torpedo_emiter->apply_color_interpolation = true; 

	new_torpedo.torpedo_emiter->initial_color[0] = in_c.r; new_torpedo.torpedo_emiter->initial_color[1] = in_c.g; new_torpedo.torpedo_emiter->initial_color[2] = in_c.b; new_torpedo.torpedo_emiter->initial_color[3] = in_c.a;
	new_torpedo.torpedo_emiter->final_color[0] = fin_c.r; new_torpedo.torpedo_emiter->final_color[1] = fin_c.g; new_torpedo.torpedo_emiter->final_color[2] = fin_c.b; new_torpedo.torpedo_emiter->final_color[3] = fin_c.r;

	new_torpedo.torpedo_emiter->gravity = { 0, -0.3f, 0 }; 

	new_torpedo.torpedo_particle = new_torpedo.torpedo_emiter->CreateParticle();

	return new_torpedo;
}

PalmTree Firework::SpawnPalmTree()
{
	PalmTree new_palmtree;

	return new_palmtree;
}
