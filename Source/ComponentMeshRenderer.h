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
	ComponentMeshRenderer(uint num_ver, float* ver, uint num_ind, uint* ind, uint num_uv = 0, float* uv = nullptr, uint num_norm = 0, float* norm = nullptr);

	~ComponentMeshRenderer();

	bool Enable(); 
	bool Update(); 
	bool Disable(); 
	void CleanUp();

	void SetCubeVertices(float3 origin, uint edge_size);
	void SetSphereVertices(float radius, uint rings, uint sectors, float3 origin);
	void SetCylinderVertices(float r, int sides, int height);
	uint GetTriNum() const;

	void SetBBox(AABB _box);
	AABB GetBBox() const;
	bool PrintBB(); 

	int GetNumVertices()const;
	uint GetVerticesID()const;
	vec* GetVertices()const;

	int GetNumIndices()const;
	uint GetIndicesID()const;
	uint* GetIndices()const;

	int GetNumUVS()const;
	uint GetUVSID()const;
	float* GetUVS()const;

	void SetNumVertices(int num);
	void SetVerticesId(uint id);
	void SetVertices(vec* vec);

	void SetNumIndices(int num);
	void SetIndicesId(uint id);
	void SetIndices(uint* ind);

	void SetNumUVS(int num);
	void SetUVSId(uint id);
	void SetUVS(float* uvs);

	bool IsBBoxShowing()const;
	bool show_bb;

private: 

	vec* vertices;
	int num_vertices = 0;
	uint vertices_id = 0;

	uint* indices;
	int num_indices = 0;
	uint indices_id = 0;

	float* uvs;
	int num_uvs = 0;
	uint uvs_id = 0;

	// As this motor can have infinite meshes per gameobject we must set an ID for apliying to 
	// each mesh its corresponding transformation

	uint tranform_id = 0; 

	AABB bounding_box;

private:

	mesh_shape shape_mesh; 

};

#endif
