#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <string>
#include<list>

#include "Bullet/include/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, 0.0f, 0.0f) 

class DebugDrawer;
//struct PhysBody3D;
//struct PhysVehicle3D;
struct VehicleInfo;
struct iPoint; 
class Gizmo; 
class Cube2; 
class GLSphere; 
class GLCylinder;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();


	/*void AddConstraintP2P(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB);
	void AddConstraintHinge(PhysBody3D& bodyA, PhysBody3D& bodyB, const vec3& anchorA, const vec3& anchorB, const vec3& axisS, const vec3& axisB, bool disable_collision = false);
*/
	std::vector<iPoint> GetCollisions() const; 

	// 3DEngine

	void PrintConfigData();

	/*Cube2* AddCube(float3 origin, uint size); 
	GLSphere* AddSphere(float3 origin, float radius, uint rings, uint sectors);
	GLCylinder* AddCylinder(float3 origin, float radius, float height, int sides);*/


private:

	bool debug;

	//btDefaultCollisionConfiguration*	collision_conf;
	//btCollisionDispatcher*				dispatcher;
	//btBroadphaseInterface*				broad_phase;
	//btSequentialImpulseConstraintSolver* solver;
	//btDiscreteDynamicsWorld*			world;
	//btDefaultVehicleRaycaster*			vehicle_raycaster;
	//DebugDrawer*						debug_draw;

	// GeoMath

	std::vector<Sphere> spheres_list; 
	std::vector<AABB> cube_list; 

	PPlane main_plane; 

	std::list<Gizmo> gizmo_list; 

	int object_type = -1;
	bool create_on_wire = false;

	vec3 curr_input_position; 
	vec3 curr_input_size; 
	int  curr_input_radium = -1;

	uint my_id;
};

class DebugDrawer : public btIDebugDraw
{
public:
	/*DebugDrawer();*/

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	//Line line;
};