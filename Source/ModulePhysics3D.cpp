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

	my_id = 0;
	glGenBuffers(1, (GLuint*)&my_id);

	glBindBuffer(GL_ARRAY_BUFFER, my_id);

	float3 origin(10.0f, 10.0f, 10.0f);

	float vertices[NUM_VERTICES * 3] =
	{
		origin.x + 1.0f, origin.y, origin.z,
		origin.x, origin.y, origin.z,
		origin.x, origin.y + 1, origin.z,

		origin.x, origin.y + 1, origin.z,
		origin.x + 1, origin.y + 1, origin.z,
		origin.x + 1, origin.y, origin.z,

		origin.x + 1, origin.y + 1, origin.z,
		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y, origin.z,

		origin.x + 1, origin.y, origin.z,
		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y, origin.z + 1,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x, origin.y + 1, origin.z + 1,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x, origin.y + 1, origin.z + 1,
		origin.x, origin.y, origin.z + 1,

		origin.x, origin.y + 1, origin.z + 1,
		origin.x, origin.y, origin.z,
		origin.x, origin.y, origin.z + 1,

		origin.x, origin.y + 1, origin.z + 1,
		origin.x, origin.y + 1, origin.z,
		origin.x, origin.y, origin.z,

		origin.x, origin.y + 1, origin.z,
		origin.x, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y + 1, origin.z + 1,

		origin.x + 1, origin.y + 1, origin.z + 1,
		origin.x + 1, origin.y + 1, origin.z,
		origin.x, origin.y + 1, origin.z,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x, origin.y, origin.z + 1,
		origin.x, origin.y, origin.z,

		origin.x + 1, origin.y, origin.z + 1,
		origin.x, origin.y, origin.z,
		origin.x + 1, origin.y, origin.z,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * NUM_VERTICES * 3, vertices, GL_STATIC_DRAW);
	
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

	std::list<PCube>::iterator it;

	for (it = pcube_list.begin(); it != pcube_list.end(); it++)
	{
		(*it).Render();
	}

	std::list<PSphere>::iterator it2;

	for (it2 = psphere_list.begin(); it2 != psphere_list.end(); it2++)
	{
		(*it2).Render();
	}

	// -----

	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	float3 origin(1.0f, 1.0f, 1.0f);

	glVertex3f(origin.x + 1.0f, origin.y, origin.z);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(origin.x, origin.y + 1, origin.z);

	glVertex3f(origin.x, origin.y + 1, origin.z);
	glVertex3f(origin.x + 1, origin.y + 1, origin.z);
	glVertex3f(origin.x + 1, origin.y, origin.z);

	glVertex3f(origin.x + 1, origin.y + 1, origin.z);
	glVertex3f(origin.x + 1, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x + 1, origin.y, origin.z);

	glVertex3f(origin.x + 1, origin.y, origin.z);
	glVertex3f(origin.x + 1, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x + 1, origin.y, origin.z + 1);

	glVertex3f(origin.x + 1, origin.y, origin.z + 1);
	glVertex3f(origin.x + 1, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x, origin.y + 1, origin.z + 1);

	glVertex3f(origin.x + 1, origin.y, origin.z + 1);
	glVertex3f(origin.x, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x, origin.y, origin.z + 1);

	glVertex3f(origin.x, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(origin.x, origin.y, origin.z + 1);

	glVertex3f(origin.x, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x, origin.y + 1, origin.z);
	glVertex3f(origin.x, origin.y, origin.z);

	glVertex3f(origin.x, origin.y + 1, origin.z);
	glVertex3f(origin.x, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x + 1, origin.y + 1, origin.z + 1);

	glVertex3f(origin.x + 1, origin.y + 1, origin.z + 1);
	glVertex3f(origin.x + 1, origin.y + 1, origin.z);
	glVertex3f(origin.x, origin.y + 1, origin.z);

	glVertex3f(origin.x + 1, origin.y, origin.z + 1);
	glVertex3f(origin.x, origin.y, origin.z + 1);
	glVertex3f(origin.x, origin.y, origin.z);

	glVertex3f(origin.x + 1, origin.y, origin.z + 1);
	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(origin.x + 1, origin.y, origin.z);

	glEnd();


	//glEnableClientState(GL_VERTEX_ARRAY); 
	//glBindBuffer(GL_VERTEX_ARRAY, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL); 
	//
	//glDrawArrays(GL_TRIANGLES, 0, 36 * 3); 
	//glBindBuffer(GL_VERTEX_ARRAY, 0); 
	//glDisableClientState(GL_VERTEX_ARRAY);


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

	for(std::list<btTypedConstraint*>::iterator item = constraints.begin(); item != constraints.end(); item++)
	{
		world->removeConstraint((*item));
		delete (*item);
	}
	
	constraints.clear();

	for(std::list<btDefaultMotionState*>::iterator item = motions.begin(); item!=motions.end(); item++)
		delete (*item);

	motions.clear();

	for(std::list<btCollisionShape*>::iterator item = shapes.begin(); item!=shapes.end(); item++)
		delete (*item);

	shapes.clear();

	for(std::list<PhysBody3D*>::iterator item = bodies.begin(); item!=bodies.end(); item++)
		delete(*item);

	bodies.clear();

	for(std::list<PhysVehicle3D*>::iterator item = vehicles.begin(); item!=vehicles.end(); item++)
		delete (*item);

	vehicles.clear();

	delete vehicle_raycaster;
	delete world;

	return true;
}

void ModulePhysics3D::AddPCube(vec3 size, vec3 position, bool wireframe)
{
	PCube new_cube(size.x, size.y, size.z); 
	new_cube.SetPos(position.x, position.y, position.z); 
	new_cube.wire = wireframe; 

	pcube_list.push_back(new_cube); 
}

void ModulePhysics3D::AddPSphere(float radium, vec3 position, bool wireframe)
{
	PSphere new_sphere(radium);
	new_sphere.SetPos(position.x, position.y, position.z);
	new_sphere.wire = wireframe;

	psphere_list.push_back(new_sphere);
}

// ---------------------------------------------------------
//PhysBody3D* ModulePhysics3D::AddBody(const Sphere& sphere, float mass)
//{
//	btCollisionShape* colShape = new btSphereShape(sphere.r);
//	shapes.add(colShape);
//
//	btTransform startTransform;
//	startTransform.setFromOpenGLMatrix(&sphere.Transform);
//
//	btVector3 localInertia(0, 0, 0);
//	if(mass != 0.f)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	motions.add(myMotionState);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//
//	body->setUserPointer(pbody);
//	world->addRigidBody(body);
//	bodies.add(pbody);
//
//	return pbody;
//}


// ---------------------------------------------------------
//PhysBody3D* ModulePhysics3D::AddBody(const Cube& cube, float mass)
//{
//	btCollisionShape* colShape = new btBoxShape(btVector3(cube.size.x*0.5f, cube.size.y*0.5f, cube.size.z*0.5f));
//	shapes.add(colShape);
//
//	btTransform startTransform;
//	startTransform.setFromOpenGLMatrix(&cube.transform);
//
//	btVector3 localInertia(0, 0, 0);
//	if(mass != 0.f)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	motions.add(myMotionState);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//
//	body->setUserPointer(pbody);
//	world->addRigidBody(body);
//	bodies.add(pbody);
//
//	return pbody;
//}

// ---------------------------------------------------------
//PhysBody3D* ModulePhysics3D::AddBody(const Cylinder& cylinder, float mass)
//{
//	btCollisionShape* colShape = new btCylinderShapeX(btVector3(cylinder, cylinder.radius, 0.0f));
//	shapes.add(colShape);
//
//	btTransform startTransform;
//	startTransform.setFromOpenGLMatrix(&cylinder.transform);
//
//	btVector3 localInertia(0, 0, 0);
//	if(mass != 0.f)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	motions.add(myMotionState);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//
//	body->setUserPointer(pbody);
//	world->addRigidBody(body);
//	bodies.add(pbody);
//
//	return pbody;
//}

// ---------------------------------------------------------
//PhysBody3D * ModulePhysics3D::AddTorus(btVector3& pos, float inner_radius, float outer_radius, float subdivisions)
//{
//	btVector3 forward(0.0f, 0.0f, 1.0f);
//	btVector3 side((outer_radius + pos.getX()), pos.getY(), pos.getZ());
//
//	double gap = sqrt(2.0*inner_radius*inner_radius
//		- 2.0*inner_radius*inner_radius*cos((2.0*SIMD_PI) / subdivisions));
//	btCylinderShapeX * shape = new btCylinderShapeX(btVector3(btScalar(inner_radius),
//		btScalar(inner_radius),
//		btScalar((SIMD_PI / subdivisions) + 0.5*gap)));
//
//	btTransform t;
//	btCompoundShape * torus_shape = new btCompoundShape();
//
//	for (int x = 0; x < (int)subdivisions; x++)
//	{
//		btScalar angle = btScalar((x*2.0*SIMD_PI) / subdivisions);
//		btVector3 position = side.rotate(forward, angle);
//		btQuaternion q(forward, angle);
//		t.setIdentity();
//		t.setOrigin(position);
//		t.setRotation(q);
//		torus_shape->addChildShape(t, shape);
//	}
//	shapes.push_back(shape);
//	shapes.push_back(torus_shape);
//	
//	btVector3 localInertia(0, 0, 0);
//	btTransform startTransform;
//	startTransform.setIdentity();
//
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(0, myMotionState, torus_shape, localInertia);
//
//	btRigidBody* body = new btRigidBody(rbInfo);
//	PhysBody3D* pbody = new PhysBody3D(body);
//	body->setUserPointer(pbody);
//	world->addRigidBody(body);
//	bodies.push_back(pbody);
//
//
//	return pbody;
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
	constraints.push_back(p2p);
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
	constraints.push_back(hinge);
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
			ImGui::Combo("Object Type", &object_type, "Cube\0Sphere\0", 2);

			ImGui::Checkbox("Wireframe", &create_on_wire); 

			if (ImGui::Button("Create!"))
			{
				switch (object_type)
				{
				case 0:
					AddPCube(vec3(1, 1, 1), vec3(0, 0, 0), create_on_wire); 

					break;

				case 1:
					AddPSphere(2, vec3(0, 0, 0), create_on_wire);
					break;

				}
			}

			ImGui::Separator();
			ImGui::TreePop(); 
		}

	}
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


