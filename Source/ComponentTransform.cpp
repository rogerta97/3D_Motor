#include "ComponentTransform.h"

float3 ComponentTransform::GetPosition() const
{
	return position;
}

Quat ComponentTransform::GetRotation() const
{
	return rotation;
}

float3 ComponentTransform::GetScale() const
{
	return scale;
}

ComponentTransform::ComponentTransform()
{
	active = true; 
}

ComponentTransform::~ComponentTransform()
{
}


bool ComponentTransform::Update()
{


	return false;
}



void ComponentTransform::SetPosition(const float3 & _position)
{
	position = _position;
}

void ComponentTransform::SetRotation(const Quat & rotation)
{
	this->rotation = rotation;
}

void ComponentTransform::SetScale(const float3 & scale)
{
	this->scale = scale;
}