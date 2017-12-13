#include "ComponentParticleEmmiter.h"
#include "ModuleFileSystem.h"
#include "Application.h"
#include "GameObject.h"
#include "OpenGL.h"

Particle::Particle()
{
	kill_me = false;
	interpolation_timer.Start(); 
	particle_color = initial_particle_color; 
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

Color Particle::GetInitialColor() const
{
	return initial_particle_color; 
}

Color Particle::GetFinalColor() const
{
	return final_particle_color;
}

void Particle::SetInitialColor(Color color)
{
	initial_particle_color = color; 
}

void Particle::SetFinalColor(Color color)
{
	final_particle_color = color; 
}

void Particle::UpdateColor()
{
	if (!interpolate_colors)
		return; 

	//We get the number that we have to increment 
	float time_ex = interpolation_timer.Read() / 1000;
	float time_dec = interpolation_timer.Read() % 1000;
	float time = time_ex + time_dec / 1000;

	float percentage = (time / (max_particle_lifetime));

	float increment_r = color_difference[0]*percentage;
	float increment_g = color_difference[1]*percentage;
	float increment_b = color_difference[2]*percentage;
	float increment_a = color_difference[3]*percentage;
	
	particle_color.r = (initial_particle_color.r + increment_r) / 255;
	particle_color.g = (initial_particle_color.g + increment_g) / 255;
	particle_color.b = (initial_particle_color.b + increment_b) / 255;
	particle_color.a = (initial_particle_color.a + increment_a) / 255;

	//LOG("R: %f G: %f B: %f", particle_color.r, particle_color.g, particle_color.b); 

}

bool Particle::IsInterpolatingColor() const
{
	return interpolate_colors;
}

void Particle::SetInterpolatingColor(bool interpolate, Color initial_color, Color final_color)
{
	interpolate_colors = interpolate;

	if (!interpolate)
		return; 

	initial_particle_color = initial_color; 
	final_particle_color = final_color; 

	color_difference[0] = (final_particle_color.r - initial_particle_color.r); 
	color_difference[1] = (final_particle_color.g - initial_particle_color.g);
	color_difference[2] = (final_particle_color.b - initial_particle_color.b);
	color_difference[3] = (final_particle_color.a - initial_particle_color.a);
}

void Particle::Update()
{
	//Translate the particles in the necessary direction
	movement += particle_gravity*0.01f; 
	components.particle_transform->SetLocalPosition(components.particle_transform->GetLocalPosition() + movement);

	//Update the particle color in case of interpolation
	if(interpolate_colors)
	{
		UpdateColor();
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
	gravity = { 0,0,0 }; 
	apply_color_interpolation = false; 

	initial_color[0] = initial_color[1] = initial_color[2] = initial_color[3] = 0; 
	final_color[0] = final_color[1] = final_color[2] = final_color[3] = 0;

	//Create the rectangle that will be the initial emmiting area (2x2 square)
	emit_area = new ComponentMeshRenderer(gameobject);
	emit_area->SetCubeVertices({ 0,0,0 }, 2);

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

	DrawEmisionArea(show_emit_area);

	return true;
}

void ComponentParticleEmmiter::UpdateRootParticle()
{
	SetEmmisionRate(emmision_rate); 

	root_particle->SetMaxLifetime(max_lifetime); 
	root_particle->SetVelocity(velocity); 
	root_particle->SetTextureByID(curr_texture_id); 
	root_particle->SetColor(color);

	root_particle->SetInterpolatingColor(apply_color_interpolation, Color(initial_color[0], initial_color[1], initial_color[2], initial_color[3]), Color(final_color[0], final_color[1], final_color[2], final_color[3]));
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

void Particle::SetMovement(float3 mov)
{
	movement = mov; 
}

void Particle::SetGravity(float3 grav)
{
	particle_gravity = grav; 
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

	//First we get the point were the particle is gonna be instanciated
	LCG random; 
	float3 particle_pos = emit_area->bounding_box.RandomPointInside(random);

	Particle* new_particle = new Particle(); 

	//We create its transform
	new_particle->components.particle_transform = new ComponentTransform(nullptr);
	new_particle->components.particle_transform->SetLocalPosition(particle_pos);

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
	new_particle->SetGravity(gravity); 

	new_particle->SetInterpolatingColor(apply_color_interpolation, root_particle->GetInitialColor(), root_particle->GetFinalColor()); 

	float3 dds = emit_area->GetComponentParent()->transform->LocalY(); 
	new_particle->SetMovement(emit_area->GetComponentParent()->transform->LocalY()*velocity);
	
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

void ComponentParticleEmmiter::DrawEmisionArea(bool show)
{
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	glColor3f(0, 0, 255);

	emit_area->Update();

	glColor3f(255, 255, 255);
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

Particle * ComponentParticleEmmiter::GetRootParticle() const
{
	return root_particle;
}
