#include "Globals.h"
#include "Application.h"
#include "Primitive.h"
#include "ModuleRenderer3D.h"
#include "ComponentCamera.h"
#include "OpenGL.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glew/glew-2.1.0/glew32.lib")

#define STD_AMBIENT_LIGHTING 0.6f
#define STD_MATERIAL_AMBIENT 1.0f
ModuleRenderer3D::ModuleRenderer3D(bool start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(json_file* config)
{
	name = "Render";
	App->performance.InitTimer(name);

	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Init glew
		GLenum glew = glewInit(); 

		LOG("Using Glew version %s", glewGetString(GLEW_VERSION));
		LOG("Renderer: %s", glGetString(GL_RENDERER));
		LOG("OpenGL version supported: %s", glGetString(GL_VERSION));
		LOG("GLSL: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Use Vsync
		if (vsync_on)
		{
			if (SDL_GL_SetSwapInterval(1) < 0)
				LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}
			
		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.6f, 0.6f, 0.6f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D); 
		
	}

	tex_loader.Start();

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	App->performance.SaveInitData(name); 
	
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	//TODO when we have more cameras assign the current one
	//now we will assign the basic editor cam

	// Adjust projection if needed
	if (rendering_cam->screen_resized == true)
	{
		ChangeRenderView();
		rendering_cam->screen_resized = false;
	}

	App->performance.InitTimer(name); 

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(rendering_cam->GetOpenGLViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);	

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	App->performance.PauseTimer(name); 

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{	

	App->performance.ResumeTimer(name); 

	SDL_GL_SwapWindow(App->window->window);

	App->performance.SaveRunTimeData(name); 

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(App->camera->GetFrustumProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	App->camera->GetEditorCam()->SetAspectRatio(width/height);



}
void ModuleRenderer3D::ChangeRenderView()
{
	ComponentCamera* cam = App->camera->GetEditorCam();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)cam->GetOpenGLProjectionMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void ModuleRenderer3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader(name))
	{
		ImGui::Text("Rendering options:"); ImGui::NewLine(); 

		if (ImGui::TreeNode("Depth Testing"))
		{
			ImGui::Checkbox("DEPTH_TEST", &depth_test_ch_b);

			ImGui::Separator();
			ImGui::TreePop(); 
		}

		if (ImGui::TreeNode("Lighting"))
		{
			ImGui::Checkbox("LIGHTING", &lighting_ch_b);

			if (lighting_ch_b)
			{
				GLenum light_type; 

				ImGui::Combo("Light Type", &light_editing_type, "Ambient\0Diffuse\0", 2); 

				switch (light_editing_type)
				{
				case 0:
					curr_light_data = tmp_color;
					light_type = GL_AMBIENT; 
					curr_light_num = GL_LIGHT0; 

					break;

				case 1:
					curr_light_data = tmp_color;
					light_type = GL_DIFFUSE;
					curr_light_num = GL_LIGHT0;
					break;
				}

				if (light_editing_type != -1) 
				{
					ImGui::ColorPicker4("Ambient color##4", curr_light_data, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_AlphaPreview, NULL);
				}

				if (ImGui::Button("Set Color!"))
				{

					GLfloat light_data[] = { curr_light_data[0], curr_light_data[1], curr_light_data[2], curr_light_data[3] };
					glLightfv(curr_light_num, light_type, light_data);
					glEnable(curr_light_num); 
				}
		
			}
			
			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Color Material"))
		{
			ImGui::Checkbox("COLOR_MATERIAL", &color_ch_b);

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Texture"))
		{
			ImGui::Checkbox("TEXTURE", &texture_ch_b);

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Wireframe"))
		{
			ImGui::Checkbox("WIREFRAME", &wireframe_ch_b);

			if (wireframe_ch_b)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			ImGui::Separator();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Cull Face"))
		{
			ImGui::Checkbox("CULL_FACE", &cull_face_ch_b);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Fog"))
		{
			ImGui::Checkbox("FOG", &fog_ch_b);

			if (fog_ch_b)
			{
				ImGui::SliderFloat("Density", &fog_density, 0, 0.5f);
			}

			ImGui::TreePop();
		}
		

		if (cull_face_ch_b == false) glDisable(GL_CULL_FACE);
		else glEnable(GL_CULL_FACE);

		if (depth_test_ch_b == false) glDisable(GL_DEPTH_TEST);
		else glEnable(GL_DEPTH_TEST);

		if (lighting_ch_b == false)
		{
			glDisable(GL_LIGHTING);		
		}
		else 
			glEnable(GL_LIGHTING);

		if (color_ch_b == false)
		{
			glDisable(GL_COLOR_MATERIAL);
		}
		else
		{
			glEnable(GL_COLOR_MATERIAL);
		}

		if (texture_ch_b == false) glDisable(GL_TEXTURE_2D);
		else glEnable(GL_TEXTURE_2D);

		if (wireframe_ch_b == false) //SetObjectsWireframe(false); 
		//else //SetObjectsWireframe(true);

		if (fog_ch_b == false)
		{
			glDisable(GL_FOG);
		}
		else
		{
			glEnable(GL_FOG); 
			glFogf(GL_FOG_DENSITY, fog_density); 
		}

		ImGui::Separator();
	
		ImGui::Text("Driver: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", this->GetDriver());

	}
}

void ModuleRenderer3D::SetRenderingCam(ComponentCamera * new_cam)
{
	rendering_cam = new_cam; 
}

void ModuleRenderer3D::DrawNormals(float3 * normals, float3* center_points, uint num)
{
	for (int i = 0; i < num; i++)
	{
		LineSegment segment(center_points[i], center_points[i] + normals[i]);
		DebugDraw(segment, Blue); 
	}
}

char * ModuleRenderer3D::GetGraphicsModel(const char* _module) const
{
	const GLubyte* model; 

	if(_module == "vendor")
		model = glGetString(GL_VENDOR);

	else if(_module == "model")
	    model = glGetString(GL_RENDERER);
	return (char*)model;
}

const char * ModuleRenderer3D::GetDriver()const
{
	return SDL_GetCurrentVideoDriver();
}
void ModuleRenderer3D::UI_attributes()
{

	GLfloat LightModelAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
}

void ModuleRenderer3D::CustomAttributes()
{
	if (light_model_ambient != STD_AMBIENT_LIGHTING) {
		GLfloat LightModelAmbient[] = { light_model_ambient, light_model_ambient, light_model_ambient, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	}
	if (material_ambient != STD_MATERIAL_AMBIENT) {
		GLfloat MaterialAmbient[] = { material_ambient, material_ambient, material_ambient, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	}

	depth_test_ch_b ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	cull_face_ch_b ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	lighting_ch_b ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	color_ch_b ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
	texture_ch_b ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
	(smooth_ch_b) ? glShadeModel(GL_SMOOTH) : glShadeModel(GL_FLAT);
	if (fog_ch_b)
	{
		glEnable(GL_FOG);
		glFogfv(GL_FOG_DENSITY, &fog_density);
	}
	else
	{
		glDisable(GL_FOG);
	}


}
//
//void ModuleRenderer3D::PrintBufferDataE(int* indices)
//{
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
//	glVertexPointer(3, GL_FLOAT, 0, NULL);
//
//	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//}

//void ModuleRenderer3D::SetObjectsWireframe(bool state)
//{
//
//	for (int i = 0; i < App->scene_intro->obj_list.size(); i++)
//	{
//		App->scene_intro->obj_list[i].wire = state;
//	}
//		
//}
