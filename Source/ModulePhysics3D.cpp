#include "Globals.h"
#include "Application.h"
#include "OpenGL.h"
#include "ModulePhysics3D.h"
#include "PhysBody3D.h"
#include "Primitive.h"

#define NUM_VERTICES 36

#ifdef _DEBUG
	#pragma comment (lib, "Bullet/libx86/BulletDynamics_debug.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision_debug.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath_debug.lib")
#else
	#pragma comment (lib, "Bullet/libx86/BulletDynamics.lib")
	#pragma comment (lib, "Bullet/libx86/BulletCollision.lib")
	#pragma comment (lib, "Bullet/libx86/LinearMath.lib")
#endif

ModulePhysics3D::ModulePhysics3D(bool start_enabled)
{
	debug = true;

	collision_conf = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_conf);
	broad_phase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	debug_draw = new DebugDrawer();
}

// Destructor
ModulePhysics3D::~ModulePhysics3D()
{
	delete debug_draw;
	delete solver;
	delete broad_phase;
	delete dispatcher;
	delete collision_conf;
}

// Render not available yet----------------------------------
bool ModulePhysics3D::Init()
{
	name = "Physics";
	App->performance.InitTimer(name); 

	LOG("Creating 3D Physics simulation");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	App->performance.SaveInitData(name);

	return ret;
}

// ---------------------------------------------------------
bool ModulePhysics3D::Start()
{
	LOG("Creating Physics environment");

	world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_conf);
	world->setDebugDrawer(debug_draw);
	world->setGravity(GRAVITY);
	vehicle_raycaster = new btDefaultVehicleRaycaster(world);

	// Big plane as ground
	{
		btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
		btDefaultMotionState* myMotionState = new btDefaultMotionState();
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

		btRigidBody* body = new btRigidBody(rbInfo);
		world->addRigidBody(body);
	}

	//// Creating objects 

	main_plane = PPlane(0, 1, 0, 0);
	main_plane.axis = true;

	 // Create cosas 
	
	return true;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PreUpdate(float dt)
{
	App->performance.InitTimer(name); 

	world->stepSimulation(dt, 15);

	int numManifolds = world->getDispatcher()->getNumManifolds();
	for(int i = 0; i<numManifolds; i++)
	{
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
		btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

		int numContacts = contactManifold->getNumContacts();
		if(numContacts > 0)
		{
			PhysBody3D* pbodyA = (PhysBody3D*)obA->getUserPointer();
			PhysBody3D* pbodyB = (PhysBody3D*)obB->getUserPointer();

			if(pbodyA && pbodyB)
			{
				
				for (std::list<Module*>::iterator item = pbodyA->collision_listeners.begin(); item != pbodyA->collision_listeners.end(); ++item)
				{
					(*item)->OnCollision(pbodyA, pbodyB);
				}

				

				for (std::list<Module*>::iterator item = pbodyB->collision_listeners.begin(); item != pbodyB->collision_listeners.end(); ++item)
				{
					(*item)->OnCollision(pbodyB, pbodyA);
				}
			}
		}
	}

	App->performance.PauseTimer(name);

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::Update(float dt)
{

	App->performance.ResumeTimer(name); 

	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
	{
		world->debugDrawWorld();
	}

	GetCollisions(); 

	//Provisional code

	//std::list<PCube>::iterator it;

	//for (it = pcube_list.begin(); it != pcube_list.end(); it++)
	//{
	//	(*it).Render();
	//}

	//std::list<PSphere>::iterator it2;

	//for (it2 = psphere_list.begin(); it2 != psphere_list.end(); it2++)
	//{
	//	(*it2).Render();
	//}

	// -----

	


	//glLineWidth(1.0f);

	App->performance.PauseTimer(name);

	return UPDATE_CONTINUE;
}

// ---------------------------------------------------------
update_status ModulePhysics3D::PostUpdate(float dt)
{

	App->performance.ResumeTimer(name);

	main_plane.Render(); 

	App->performance.SaveRunTimeData(name); 

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModulePhysics3D::CleanUp()
{
	LOG("Destroying 3D Physics simulation");

	// Remove from the world all collision bodies
	for(int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		world->removeCollisionObject(obj);
	}

	//for(std::list<btTypedConstraint*>::iterator item = constraints.begin(); item != constraints.end(); item++)
	//{
	//	world->removeConstraint((*item));
	//	delete (*item);
	//}
	//
	//constraints.clear();

	//for(std::list<btDefaultMotionState*>::iterator item = motions.begin(); item!=motions.end(); item++)
	//	delete (*item);

	//motions.clear();

	//for(std::list<btCollisionShape*>::iterator item = shapes.begin(); item!=shapes.end(); item++)
	//	delete (*item);

	//shapes.clear();

	//for(std::list<PhysBody3D*>::iterator item = bodies.begin(); item!=bodies.end(); item++)
	//	delete(*item);

	//bodies.clear();

	//for(std::list<PhysVehicle3D*>::iterator item = vehicles.begin(); item!=vehicles.end(); item++)
	//	delete (*item);

	//vehicles.clear();

	delete vehicle_raycaster;
	delete world;

	return true;
}

//void ModulePhysics3D::AddPCube(vec3 size, vec3 position, bool wireframe)
//{
//	PCube new_cube(size.x, size.y, size.z); 
//	new_cube.SetPos(position.x, position.y, position.z); 
//	new_cube.wire = wireframe; 
//
//	pcube_list.push_back(new_cube); 
//}
//
//void ModulePhysics3D::AddPSphere(float radium, vec3 position, bool wireframe)
//{
//	PSphere new_sphere(radium);
//	new_sphere.SetPos(position.x, position.y, position.z);
//	new_sphere.wire = wireframe;
//
//	psphere_list.push_back(new_sphere);
//}

// ---------------------------------------------------------
void ModulePhysics3D::AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB)
{
	btTypedConstraint* p2p = new btPoint2PointConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z), 
		btVector3(anchorB.x, anchorB.y, anchorB.z));
	world->addConstraint(p2p);
	p2p->setDbgDrawSize(2.0f);
}

void ModulePhysics3D::AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisA, const vec3& axisB, bool disable_collision)
{
	btHingeConstraint* hinge = new btHingeConstraint(
		*(bodyA.body), 
		*(bodyB.body), 
		btVector3(anchorA.x, anchorA.y, anchorA.z),
		btVector3(anchorB.x, anchorB.y, anchorB.z),
		btVector3(axisA.x, axisA.y, axisA.z), 
		btVector3(axisB.x, axisB.y, axisB.z));

	world->addConstraint(hinge, disable_collision);
	hinge->setDbgDrawSize(2.0f);
}

std::vector<iPoint> ModulePhysics3D::GetCollisions()
{ 
	std::vector<iPoint> tmp_obj_list;
	
	int listener = 0, candidate = 0; 
	bool empty = true; 

	for (listener; listener < spheres_list.size(); listener++)
	{
		
		//LOG("Body %d is coliding with:" , listener + 1);

		empty = true; 

		for(candidate; candidate < spheres_list.size(); candidate++)
		{ 
			if (listener == candidate) {
				continue; 
			}

			bool colision_test = false; 
			colision_test = spheres_list[listener].Intersects(spheres_list[candidate]);

			if (colision_test)
			{
				//LOG("Body %d.", candidate + 1);
				tmp_obj_list.push_back(iPoint(listener, candidate)); 
				empty = false; 
			}

		}

		if (empty)
			//LOG("This body DONT collide");

		candidate = 0; 
		
	}
	
	return tmp_obj_list; 
}

void ModulePhysics3D::PrintConfigData()
{
	if (ImGui::CollapsingHeader(name))
	{
		if (ImGui::TreeNode("Object Creator"))
		{
			ImGui::Combo("Object Type", &object_type, "Cube\0Sphere\0Cylinder\0", 3);

			ImGui::Separator(); 

			static float3 origin_vector = { 0.0f, 0.0f, 0.0f };
			static int size = 0;

			bool create_obj = false; 

			ImGui::InputFloat("X", &origin_vector.x, 0.1f, 5.0f, 2);
			ImGui::InputFloat("Y", &origin_vector.y, 0.1f, 5.0f, 2);
			ImGui::InputFloat("Z", &origin_vector.z, 0.1f, 5.0f, 2);

			ImGui::Separator(); 

			switch (object_type)
			{
			case 0:
				if (size < 0)
					size = 0; 

				ImGui::InputInt("Edge", &size, 1, 50);

				ImGui::Separator(); 

				if (ImGui::Button("Create!"))
				{
					if (size == 0)
					{
						LOG("Error crating cube, size must be +0");
						break;
					}
						
					if (!App->scene_intro->obj_list.empty())
					{
						LOG("Deleting Gizmo with buffer num %d", App->scene_intro->obj_list[0]->vertices_id);
						delete(App->scene_intro->obj_list[0]);
						App->scene_intro->obj_list.clear();
					}

					App->scene_intro->obj_list.push_back((Gizmo*)AddCube(origin_vector, size)); 
				}							
				break;

			case 1:

				static int rings; 
				static int sectors; 
				static float radius;

				if (radius < 0) radius = 0;
				if (rings < 0) rings = 0; 
				if (sectors < 0) sectors = 0; 

				ImGui::InputFloat("Radius", &radius, 1, 50, 2);
				ImGui::InputInt("Rings", &rings, 1, 50);
				ImGui::InputInt("Sectors", &sectors, 1, 50);

				ImGui::Separator();

				if (ImGui::Button("Create!"))
				{

					if (radius == 0 || rings < 2 || sectors < 2)
					{
						LOG("Radius should be +2 / Rings & sectors should be +2");
						break;
					}

					if (!App->scene_intro->obj_list.empty())
					{
						LOG("Deleting Gizmo with buffer num %d", App->scene_intro->obj_list[0]->vertices_id);
						delete(App->scene_intro->obj_list[0]);
						App->scene_intro->obj_list.clear();
					}

					App->scene_intro->obj_list.push_back((Gizmo*)AddSphere(origin_vector, radius, rings, sectors));
				}					
				break;


			case 2:
				static int height;
				static int sides;
				static float r; 

				if (r < 0) r = 0;
				if (sides < 0) sides = 0;
				if (height < 0) height = 0;

				ImGui::InputFloat("Radius", &r, 1, 50, 2);
				ImGui::InputInt("Height", &height, 1, 50);
				ImGui::InputInt("Sides", &sides, 1, 50);

				ImGui::Separator();

				if (ImGui::Button("Create!"))
				{

					if (r == 0 || height == 0 || sides == 0)
					{
						LOG("All properties must be +0"); 
						break;
					}
						
					if (!App->scene_intro->obj_list.empty())
					{
						LOG("Deleting Gizmo with buffer num %d", App->scene_intro->obj_list[0]->vertices_id);
						delete(App->scene_intro->obj_list[0]);
						App->scene_intro->obj_list.clear();
					}

					App->scene_intro->obj_list.push_back((Gizmo*)AddCylinder(origin_vector, r, height, sides));
				}								
				break;
			}
			ImGui::TreePop(); 
		}
	}
}

Cube2* ModulePhysics3D::AddCube(float3 origin, uint size)
{
	Cube2* new_cube = new Cube2();

	new_cube->Start(origin, (float)size);

	return new_cube; 
}

GLSphere* ModulePhysics3D::AddSphere(float3 origin, float radius, uint rings, uint sectors)
{
	GLSphere* new_sphere = new GLSphere();

	new_sphere->Start(radius, rings, sectors, origin);

	return new_sphere;
}

GLCylinder* ModulePhysics3D::AddCylinder(float3 origin, float radius, float height, int sides)
{
	GLCylinder* new_cylinder = new GLCylinder();

	new_cylinder->Start(radius, sides, height);

	return new_cylinder;
}

// =============================================
void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	/*line.origin.Set(from.getX(), from.getY(), from.getZ());
	line.destination.Set(to.getX(), to.getY(), to.getZ());
	line.color.Set(color.getX(), color.getY(), color.getZ());
	line.Render();*/
}

void DebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	//point.transform.translate(PointOnB.getX(), PointOnB.getY(), PointOnB.getZ());
	//point.color.Set(color.getX(), color.getY(), color.getZ());
	//point.Render();
}

void DebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("Bullet warning: %s", warningString);
}

void DebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	LOG("Bullet draw text: %s", textString);
}

void DebugDrawer::setDebugMode(int debugMode)
{
	mode = (DebugDrawModes) debugMode;
}

int	 DebugDrawer::getDebugMode() const
{
	return mode;
}


