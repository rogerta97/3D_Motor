#include "ComponentParticleEmmiter.h"
#include "ModuleFileSystem.h"
#include "Application.h"
#include "OpenGL.h"

Particle::Particle()
{
	particle_timer.Start(); 
	material_shape = MAT_SHAPE_SQUARE; 
}

void Particle::GenerateParticle()
{
	atributes.particle_pos->SetLocalPosition(atributes.particle_pos->GetLocalPosition() + float3(0,1,0));
}

void Particle::Update()
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
	emmision_frequency = 0.5f; 
	emmision_rate = 1; 

	particles_lifetime = 0.0f; 
	system_state = PARTICLE_STATE_PAUSE;

	shapes_amount = 0; 

}

void ComponentParticleEmmiter::Start()
{
	//Create the rectangle that will be the initial emmiting area (2x2 square)
	emit_area = new ComponentMeshRenderer(gameobject); 
	emit_area->SetPlaneVertices({0,0,0}, 2);

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

}

bool ComponentParticleEmmiter::Update()
{

	if (!active)
		return false; 
	
	DrawEmisionArea(); 
	
	return true;
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

particle_system_state ComponentParticleEmmiter::GetSystemState() const
{
	return system_state;
}

void ComponentParticleEmmiter::SetSystemState(particle_system_state new_state)
{
	system_state = new_state; 
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
