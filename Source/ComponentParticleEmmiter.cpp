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
	return components;
}

void Particle::SetMaxLifetime(const float& new_lifetime)
{
	max_particle_lifetime = new_lifetime; 
}

float Particle::GetMaxLifeTime() const
{
	return max_particle_lifetime;
}

void Particle::SetVelocity(const float & new_velocity)
{
	particle_velocity = new_velocity; 
}

float Particle::GetVelocity() const
{
	return particle_velocity;
}

void Particle::SetTextureByID(uint texture_ID)
{
	particle_texture_id = texture_ID;
}

void Particle::SetColor(Color new_color)
{
	particle_color = new_color; 
}

Color Particle::GetColor() const
{
	return particle_color;
}

void Particle::Update()
{

	//Update V position
	float3 movement = {0, particle_velocity, 0};
	components.particle_transform->SetLocalPosition(components.particle_transform->GetLocalPosition() + movement);

	//Update Billboarding rotation
	if (components.particle_billboarding != nullptr)
	{
		components.particle_billboarding->Update(); 
	}

	//Check if they have to be deleted
	if (particle_timer.Read() > max_particle_lifetime*1000)
		kill_me = true; 
}

void Particle::Delete()
{
	components.particle_mesh->Delete();
	components.particle_material->Delete();
	components.particle_billboarding->Delete();

	components.SetToNull();
}

void Particle::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(particle_color.r, particle_color.g, particle_color.b);

	glPushMatrix();
	glMultMatrixf(components.particle_transform->GetLocalTransform().Transposed().ptr());

	glBindBuffer(GL_ARRAY_BUFFER, components.particle_mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (particle_texture_id != -1)
	{
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindTexture(GL_TEXTURE_2D, particle_texture_id); 

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, components.particle_mesh->uvs_id);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, components.particle_mesh->indices_id);

	glDrawElements(GL_TRIANGLES, components.particle_mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3f(255,255,255);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();
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
	velocity = 0.5f; 
	curr_texture_id = -1; 
	color = Color(255, 255, 255, 0); 
	show_billboarding = false; 

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
	root_particle->components.SetToNull();

	root_particle->SetMaxLifetime(max_lifetime); 
	root_particle->SetVelocity(velocity);
	root_particle->SetTextureByID(-1); 
}

bool ComponentParticleEmmiter::Update()
{
	if (!active)
		return false; 
	
	if(show_emit_area)	DrawEmisionArea();

	//Create particles if needed
	GenerateParticles(); 

	//Update & Draw particles 
	for (list<Particle*>::iterator it = active_particles.begin(); it != active_particles.end(); it++)
	{
		if ((*it)->IsDead())
		{
			(*it)->Delete(); 
			it = active_particles.erase(it); 
			LOG("PARTICLE DELETED"); 

			if (active_particles.size() > 0)
				continue;
			else
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
	root_particle->SetVelocity(velocity); 
	root_particle->SetTextureByID(curr_texture_id); 
	root_particle->SetColor(color);
}

ComponentParticleEmmiter::~ComponentParticleEmmiter()
{
}

void Particle::SetBillboardReference(ComponentCamera* new_reference)
{
	components.particle_billboarding->SetReference(new_reference); 
}

ComponentCamera * Particle::GetBillboardReference()
{
	return components.particle_billboarding->GetReference();
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
	new_particle->components.particle_transform = new ComponentTransform(nullptr);

	//We generate the always squared surface for the particle 
	new_particle->components.particle_mesh = new ComponentMeshRenderer(nullptr);
	new_particle->components.particle_mesh->SetPlaneVertices({ gameobject->transform->GetLocalPosition().x, gameobject->transform->GetLocalPosition().y, gameobject->transform->GetLocalPosition().z }, 2);

	//Billboard the squad for always be looking at the camera, at the beggining it will be deactivated 
	if (show_billboarding)
	{
		new_particle->components.particle_billboarding = new ComponentBillboarding(nullptr, new_particle);
		new_particle->SetBillboardReference(App->renderer3D->rendering_cam);

		if (lock_billboarding_y)
			new_particle->components.particle_billboarding->LockY(); 
		else
			new_particle->components.particle_billboarding->UnLockY();

		if (lock_billboarding_x)
			new_particle->components.particle_billboarding->LockX(); 
		else
			new_particle->components.particle_billboarding->UnLockX();
	}
		
	new_particle->SetMaxLifetime(max_lifetime); 
	new_particle->SetVelocity(velocity); 
	new_particle->SetTextureByID(curr_texture_id);
	new_particle->SetColor(color); 
	
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

void ComponentParticleEmmiter::SetCurrentTextureID(uint texture_id)
{
	curr_texture_id = texture_id; 
}

uint ComponentParticleEmmiter::GetCurrentTextureID() const
{
	return curr_texture_id;
}
