#include "ComponentParticleEmmiter.h"
#include "ModuleFileSystem.h"
#include "Application.h"
#include "GameObject.h"
#include "OpenGL.h"

Particle::Particle()
{
	kill_me = false;
}

ParticleComponents Particle::GetAtributes()
{
	return atributes;
}

void Particle::SetMaxLifetime(float new_lifetime)
{
	max_particle_lifetime = new_lifetime; 
}

float Particle::GetMaxLifeTime()
{
	return max_particle_lifetime;
}

void Particle::Update()
{
	atributes.particle_transform->SetLocalPosition(atributes.particle_transform->GetLocalPosition() + float3(0,0.2f,0));

	if (particle_timer.Read() > max_particle_lifetime*1000)
		kill_me = true; 
}

void Particle::Delete()
{
	atributes.particle_mesh->Delete(); 
	atributes.particle_material->Delete(); 
	atributes.particle_billboarding->Delete(); 

	atributes.SetToNull(); 
}

void Particle::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glPushMatrix();
	glMultMatrixf(atributes.particle_transform->GetLocalTransform().Transposed().ptr());

	glBindBuffer(GL_ARRAY_BUFFER, atributes.particle_mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, atributes.particle_mesh->indices_id);
	glDrawElements(GL_TRIANGLES, atributes.particle_mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
}

void Particle::DrawMesh()
{



}

Particle::~Particle()
{
}

ComponentParticleEmmiter::ComponentParticleEmmiter(GameObject* parent)
{
	//Component properties
	gameobject = parent;
	active = true;
	type = COMPONENT_PARTICLE_EMMITER;

	//Emmiter properties
	emmision_frequency = 1000; 
	emmision_rate = 1; 

	particles_lifetime = 0.0f; 
	system_state = PARTICLE_STATE_PAUSE;

	shapes_amount = 0; 
	emmision_rate = 1; 
	max_lifetime = 1; 

	//Create the rectangle that will be the initial emmiting area (2x2 square)
	emit_area = new ComponentMeshRenderer(gameobject);
	emit_area->SetPlaneVertices({ 0,0,0 }, 2, false);

	//Getting the default shapes 
	string particles_folder_path = App->file_system->particles_path_game;
	vector<string> images = App->file_system->GetFilesInDirectory(particles_folder_path.c_str(), "png");

	//Setting the id's of the images into the list, if there are
	for (int i = 0; i < images.size(); i++)
	{
		string path_to_load = particles_folder_path + images[i];

		ComponentMaterial* current_image = App->resource_manager->material_loader->ImportImage(path_to_load.c_str());

		if (current_image != nullptr)
		{
			shapes_ids.push_back(current_image->textures_id);
			shapes_amount++;
		}

		delete(current_image);
		current_image = nullptr;
	}

	//This is the root particle that we are going to clone
	CreateRootParticle(); 

}

void ComponentParticleEmmiter::Start()
{	
	spawn_timer.Start(); 
}

void ComponentParticleEmmiter::CreateRootParticle()
{
	root_particle = new Particle(); 
	root_particle->atributes.SetToNull();

	root_particle->SetMaxLifetime(max_lifetime); 
}

bool ComponentParticleEmmiter::Update()
{
	if (!active)
		return false; 
	
	if(show_emit_area)	DrawEmisionArea();

	GenerateParticles(); 

	for (list<Particle*>::iterator it = active_particles.begin(); it != active_particles.end(); it++)
	{
		if ((*it)->IsDead())
		{
			(*it)->Delete(); 
			it = active_particles.erase(it); 
			LOG("PARTICLE DELETED"); 
			break; 
		}

		(*it)->Update();
		(*it)->Draw();
	}

	return true;
}

void ComponentParticleEmmiter::UpdateRootParticle()
{
	SetEmmisionRate(emmision_rate); 
	root_particle->SetMaxLifetime(max_lifetime); 
}

ComponentParticleEmmiter::~ComponentParticleEmmiter()
{
}

void ComponentParticleEmmiter::GenerateParticles()
{
	if (system_state == PARTICLE_STATE_PAUSE)
		return; 

	if (spawn_timer.Read() > emmision_frequency)
	{		
		Particle* new_particle = CreateParticle(); 
		active_particles.push_back(new_particle);
		LOG("Particles ammount: %d", GetParticlesNum()); 
		spawn_timer.Start(); 
	}

}

Particle * ComponentParticleEmmiter::CreateParticle()
{
	Particle* new_particle = new Particle(); 

	//We create its transform
	new_particle->atributes.particle_transform = new ComponentTransform(nullptr);

	//We generate the always squared surface for the particle 
	new_particle->atributes.particle_mesh = new ComponentMeshRenderer(nullptr);
	new_particle->atributes.particle_mesh->SetPlaneVertices({ gameobject->transform->GetLocalPosition().x, gameobject->transform->GetLocalPosition().y, gameobject->transform->GetLocalPosition().z }, 2);

	new_particle->SetMaxLifetime(max_lifetime); 
	
	return new_particle; 
}

int ComponentParticleEmmiter::GetParticlesNum()
{
	return active_particles.size(); 
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
		emmision_frequency = (1000 / rate);
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

particle_system_state ComponentParticleEmmiter::GetSystemState() const
{
	return system_state;
}

void ComponentParticleEmmiter::SetSystemState(particle_system_state new_state)
{
	system_state = new_state; 
}

bool Particle::IsDead()
{
	return kill_me;
}

void ComponentParticleEmmiter::DrawEmisionArea()
{
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f); 
	glColor3f( 0, 0, 255 ); 

	emit_area->Update();

	glColor3f( 255, 255, 255 );
	glLineWidth(1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int ComponentParticleEmmiter::GetTextureID(int pos)
{
	return shapes_ids[pos];
}

int ComponentParticleEmmiter::GetTextureIDAmount()
{
	return shapes_amount; 
}
