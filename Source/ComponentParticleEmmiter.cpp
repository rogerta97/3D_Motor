#include "ComponentParticleEmmiter.h"
#include "ModuleFileSystem.h"
#include "Application.h"
#include "Functions.h"
#include "GameObject.h"
#include "OpenGL.h"
#include <map>

Particle::Particle()
{
	kill_me = false;
	interpolation_timer.Start(); 
	particle_color = initial_particle_color; 
	particle_angular_v = 0; 
	curr_rot = 0; 
	angular_v_increment = 0; 
	animated_particle = false; 
	interpolate_size = false; 
	interpolate_rotation = false; 
	animation_timer.Start(); 
	twister.Start(); 
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

void Particle::SetAngular(const float & new_velocity)
{
	particle_angular_v = new_velocity; 
}

float Particle::GetAngular() const
{
	return particle_angular_v;
}

void Particle::SetRotationIncrement(float increment)
{
	angular_v_increment = increment; 
}

float Particle::GetRotationIncrement()
{
	return angular_v_increment;
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

void Particle::SetDistanceToCamera(float new_dist)
{
	distance_to_camera = new_dist; 
}

float Particle::GetDistanceToCamera()
{
	float distance = (App->renderer3D->rendering_cam->frustum.pos - components.particle_transform->GetLocalPosition()).Length(); 
	distance_to_camera = distance; 

	return distance_to_camera;
}

void Particle::SetBillboarding(float new_dist)
{
	billboarding = new_dist;
}

float Particle::IsBillboarding() const
{
	return billboarding;
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

void Particle::UpdateSize()
{
	if (!interpolate_size)
		return;

	//We get the number that we have to increment 
	float time_ex = interpolation_timer.Read() / 1000;
	float time_dec = interpolation_timer.Read() % 1000;
	float time = time_ex + time_dec / 1000;

	float percentage = (time / (max_particle_lifetime));

	float3 total_increment((final_particle_size.x - initial_particle_size.x), (final_particle_size.y - initial_particle_size.y), 1);

	float increment_x = total_increment.x * percentage;
	float increment_y = total_increment.y * percentage;
	float increment_z = 1;

	float3 new_scale = { initial_particle_size.x + increment_x, initial_particle_size.y + increment_y, 1 };

	components.particle_transform->SetLocalScale(new_scale);

}

void Particle::SetInterpolationSize(bool interpolate, float3 initial_scale, float3 final_scale)
{
	interpolate_size = interpolate;

	initial_particle_size = initial_scale;
	final_particle_size = final_scale;
}

void Particle::Update()
{

	//Translate the particles in the necessary direction
	movement += particle_gravity*0.01f; 
	components.particle_transform->SetLocalPosition(components.particle_transform->GetLocalPosition() + movement);

	//Update the particle color in case of interpolation
	if(interpolate_colors) 
		UpdateColor();
	
	//Update scale
	if (interpolate_size)
		UpdateSize(); 

	//Update Rotation	
	UpdateRotation();

	//Update Billboarding
	if(IsBillboarding() == true)
		components.particle_billboarding->Update(); 

	//Animations
	if(animated_particle)
		UpdateAnimation(); 
	
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

	//Emmiter properties -------

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
	billboarding = false; 
	gravity = { 0,0,0 }; 
	angular_v = 0; 
	emision_angle = 0; 
	reorder_time.Start(); 
	is_animated = false; 
	time_step = 0.2; 

	apply_rotation_interpolation = false; 
	apply_size_interpolation = false; 
	apply_color_interpolation = false; 

	initial_scale = { 1,1,1 }; 
	final_scale = { 1,1,1 };

	initial_angular_v = 0; 
	final_angular_v = 0; 

	initial_color[0] = initial_color[1] = initial_color[2] = initial_color[3] = 0; 
	final_color[0] = final_color[1] = final_color[2] = final_color[3] = 0;

	// ------

	//Create the rectangle that will be the initial emmiting area (2x2 square)
	emit_area = new ComponentMeshRenderer(gameobject);
	emit_area->SetCubeVertices({ 0,0,0 }, 2);

	//Getting the default shapes 
	string particles_folder_path = App->file_system->particles_path_game;
	vector<string> images = App->file_system->GetFilesInDirectory(particles_folder_path.c_str(), "png");

	//Setting the id's of the images into the list, if there are
	//for (int i = 0; i < images.size(); i++)
	//{
	//	string path_to_load = particles_folder_path + images[i];

	//	ComponentMaterial* current_image = App->resource_manager->material_loader->ImportImage(path_to_load.c_str());

	//	if (current_image != nullptr)
	//	{
	//		shapes_ids.push_back(current_image->textures_id);
	//		shapes_amount++;
	//	}

	//	delete(current_image);
	//	current_image = nullptr;
	//}

	//This is the root particle that we are going to clone
	LoadParticleImages();
	LoadParticleAnimations();

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

			if (active_particles.size() > 0)
				continue;
			else
				break; 
		}

		(*it)->Update();
		particles_sorted.insert(pair<float, Particle*>((*it)->GetDistanceToCamera(),(*it)));
		
	}

	////We first order the particles based on the dist to camera
	for (auto it = particles_sorted.rbegin(); it != particles_sorted.rend(); ++it)
	{	
		it->second->Draw();
	}
	
	particles_sorted.clear(); 


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
	root_particle->SetInterpolationSize(apply_size_interpolation, initial_scale, final_scale);
	root_particle->SetBillboarding(true); 

	if (apply_rotation_interpolation)
	{
		root_particle->SetInterpolationRotation(true, initial_angular_v, final_angular_v);
		root_particle->SetRotationIncrement((final_angular_v - initial_angular_v));
	}
				
	else
		root_particle->SetAngular(angular_v);

	if(apply_size_interpolation) root_particle->SetInterpolationSize(true, initial_scale, final_scale);
}

void ComponentParticleEmmiter::SetSmokeRoot()
{
	root_particle = new Particle();
	root_particle->components.SetToNull();

	SetEmmisionRate(20); 
	max_lifetime = 3.60f;
	velocity = 0.164;
	billboarding = true; 

	LoadParticleImages(); 
	curr_texture_id = particle_images_bffs[0];
	gravity = { 0.1f, 0.1f, 0 };

	apply_color_interpolation = true;

	Color initial(255, 255, 0, 0);
	Color final(255, 0, 0, 0);

	GetRootParticle()->SetInitialColor(initial);
	GetRootParticle()->SetFinalColor(final);

	apply_size_interpolation = true;

	initial_scale = { 1,1,1 };
	final_scale = { 5,5,5 };
	
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

void Particle::UpdateAnimation()
{
	if (animation_timer.Read() > components.particle_animation.timeStep * 1000)
	{		
		SetTextureByID(components.particle_animation.buffer_ids[components.particle_animation.Update(animation_timer)]);
		animation_timer.Start(); 
	}
}

void Particle::UpdateRotation()
{

	//We get the number that we have to increment 
	float time_ex = interpolation_timer.Read() / 1000;
	float time_dec = interpolation_timer.Read() % 1000;
	float time = time_ex + time_dec / 1000;

	float percentage = (time / (max_particle_lifetime));

	//if (twister.Read() > 10)
	//{
		if (interpolate_rotation)
		{
			float curr_angular_v = initial_particle_angular_v + (angular_v_increment*percentage);
			components.particle_transform->SetLocalRotation({ 0,0, curr_angular_v });

			if (curr_rot > 360)
				curr_rot = 0;

			return;
		}
		else if(particle_angular_v)
			components.particle_transform->SetLocalRotation({ 0,0, particle_angular_v });

		twister.Start();
	//}

	//float increment_v = final_particle_angular_v - initial_particle_angular_v; 

	//curr_rot += initial_particle_angular_v + (increment_v*percentage);

	//if (curr_rot > 360)
	//	curr_rot = 0;

	//components.particle_transform->SetLocalRotation({ 0,0,curr_rot });


}

void Particle::SetInterpolationRotation(bool interpolate_rot, float initial_v, float final_v)
{
	interpolate_rotation = interpolate_rot;

	initial_particle_angular_v = initial_v; 
	final_particle_angular_v = final_v;
}

void Particle::SetMovement(float3 mov)
{
	movement = mov / 7; 
}

float3 Particle::GetMovement()
{
	return movement;
}

void Particle::SetGravity(float3 grav)
{
	particle_gravity = grav / 5; 
}


void ComponentParticleEmmiter::GenerateParticles()
{
	if (system_state == PARTICLE_STATE_PAUSE)
		return; 

	if (spawn_timer.Read() > emmision_frequency)
	{		
		Particle* new_particle = CreateParticle(); 
		new_particle->SetDistanceToCamera(0);
		active_particles.push_back(new_particle);
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
	if (billboarding)
	{
		new_particle->SetBillboarding(true); 
		new_particle->components.particle_billboarding = new ComponentBillboarding(nullptr, new_particle);
		new_particle->SetBillboardReference(App->renderer3D->rendering_cam);
		new_particle->components.particle_billboarding->LockY(); 
	}
		
	new_particle->SetMaxLifetime(max_lifetime); 
	new_particle->SetVelocity(velocity); 
	new_particle->SetTextureByID(curr_texture_id);
	new_particle->SetColor(color);  
	new_particle->SetGravity(gravity); 
	new_particle->SetDistanceToCamera(0); 

	new_particle->SetInterpolatingColor(apply_color_interpolation, root_particle->GetInitialColor(), root_particle->GetFinalColor()); 

	if(apply_size_interpolation)
		new_particle->SetInterpolationSize(apply_size_interpolation, initial_scale, final_scale);
	else
		new_particle->SetInterpolationSize(apply_size_interpolation, { 1,1,1 }, {1,1,1});

	if (apply_rotation_interpolation)
	{
		new_particle->SetInterpolationRotation(true, initial_angular_v, final_angular_v);
		new_particle->SetRotationIncrement(root_particle->GetRotationIncrement()); 
	}
		
	else
	{
		new_particle->SetInterpolationRotation(false, 0, 0);
		new_particle->SetAngular(angular_v);
	}

	if (is_animated)
	{
		new_particle->animated_particle = true; 
		new_particle->components.particle_animation = root_particle->components.particle_animation; 
		new_particle->components.particle_animation.timeStep = time_step; 
		new_particle->SetTextureByID(new_particle->components.particle_animation.buffer_ids[0]);
	}
	
	float3 dds = emit_area->GetComponentParent()->transform->LocalY(); 

	if (emision_angle > 0)
	{
		//First we generate a random number between 0 and 360 that will be the X direction
		int emision_x = random.Int(1, 180);

		//This will be the angle that the particle will have, random between 0 & max angle 
		int emision_y = random.Int(1, emision_angle);

		////From those 2, we get the final direction 
		float3 final_direction = emit_area->GetComponentParent()->transform->LocalY();

		//First we rotate around x
		float3x3 y_rot_mat  = float3x3::FromEulerXYZ(emision_y, 0, 0);
		final_direction = y_rot_mat.Transform(final_direction);

		//Then we rotate around y
		float3x3 x_rot_mat = float3x3::FromEulerXYZ(0, emision_x, 0);
		final_direction = x_rot_mat.Transform(final_direction);

		new_particle->SetMovement(final_direction); 

	}
	else
		new_particle->SetMovement(emit_area->GetComponentParent()->transform->LocalY()*velocity);

	return new_particle; 
}

void ComponentParticleEmmiter::BumbParticle()
{
	if (system_state == PARTICLE_STATE_PAUSE)
		return;

	if (spawn_timer.Read() > emmision_frequency)
	{
		Particle* new_particle = CreateParticle();
		new_particle->SetDistanceToCamera(0);
		active_particles.push_back(new_particle);
		spawn_timer.Start();
		system_state = PARTICLE_STATE_PAUSE;
	}
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
	return particle_images_bffs[pos];
}

int ComponentParticleEmmiter::GetTextureIDAmount()
{
	return particle_images_bffs.size();
}

void ComponentParticleEmmiter::SetCurrentTextureID(uint texture_id)
{
	curr_texture_id = texture_id; 
}

uint ComponentParticleEmmiter::GetCurrentTextureID() const
{
	return curr_texture_id;
}

void ComponentParticleEmmiter::LoadParticleAnimations()
{
	//Load the animated particles of the engine by default

	std::string path(App->file_system->particles_path_game);

	vector<string> animation_folders = App->file_system->GetFoldersInDirectory(path.c_str());

	for (int i = 0; i < animation_folders.size(); i++)
	{
		ParticleAnimation new_particle_anim;
		new_particle_anim.name = animation_folders[i];

		string folder_path(path + new_particle_anim.name + '\\'); 
	
		vector<string> anim_set = App->file_system->GetFilesInDirectory(folder_path.c_str(), "png");

		//Load the images and add it to the buffer 
		for (int j = 0; j < anim_set.size(); j++)
		{
			string image_path(folder_path + anim_set[j]);
			ComponentMaterial* image_cmp = App->resource_manager->material_loader->ImportImage(image_path.c_str());
			new_particle_anim.buffer_ids.push_back(image_cmp->textures_id);
		}	

		particle_animations.push_back(new_particle_anim); 
	}
}

void ComponentParticleEmmiter::LoadParticleImages()
{
	//Load the animated particles of the engine by default

	particle_images_bffs.clear(); 

	std::string path(App->file_system->particles_path_game);

	vector<string> particle_images = App->file_system->GetFilesInDirectory(path.c_str(), "png");

	for (int i = 0; i < particle_images.size(); i++)
	{
		string image_path(path + particle_images[i]); 

		ComponentMaterial* image_cmp = App->resource_manager->material_loader->ImportImage(image_path.c_str());
		particle_images_bffs.push_back(image_cmp->textures_id);
	}
}


vector<ParticleAnimation> ComponentParticleEmmiter::GetAllParticleAnimations()
{
	return particle_animations;
}

Particle * ComponentParticleEmmiter::GetRootParticle() const
{
	return root_particle;
}
