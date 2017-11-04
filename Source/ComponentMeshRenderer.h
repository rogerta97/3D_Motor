#pragma once

#ifndef _COMPONENTMESHRENDERER_H_
#define _COMPONENTMESHRENDERER_H_

#include "Globals.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Component.h"

#define M_PI 3.14
#define M_PI_2 M_PI/2

class ComponentMeshRenderer : public Component
{

public: 
	ComponentMeshRenderer(GameObject* parent); 

	~ComponentMeshRenderer();

	bool Enable(); 
	bool Update(); 
	bool Disable(); 

	void SetCubeVertices(float3 origin, uint edge_size);
	void SetSphereVertices(float radius, uint rings, uint sectors, float3 origin);
	void SetCylinderVertices(float r, int sides, int height);
	uint GetTriNum() const;

	void SetGizmoBox(AABB _box);
	AABB GetGizmoBox() const;
	bool PrintBB(); 

public: 

	vec* vertices;
	int num_vertices = 0;

	uint* indices;
	int num_indices = 0;

	float* uvs;
	int num_uvs = 0;

	uint vertices_id = 0;
	uint uvs_id = 0;
	uint indices_id = 0;

	// As this motor can have infinite meshes per gameobject we must set an ID for apliying to 
	// each mesh its corresponding transformation

	uint tranform_id = 0; 

	AABB bounding_box;
	bool show_bb;

private:

	mesh_shape shape_mesh; 

};

#endif
