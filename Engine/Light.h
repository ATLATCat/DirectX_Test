#pragma once

#include "Component.h"

namespace ATLAT
{

struct LightData
{
	LightData() { ZeroMemory(this, sizeof(this)); }

	Vector4 diffuse;
	Vector4 ambient;
	Vector4 specular;
	Vector3 direction;
	float spot;
	Vector3 position;
	float range;
	Vector3 attenuation;
	float pad;
};


class Light : public Component
{
public:
	Light();
	virtual ~Light();

	virtual void Awake() override;
	virtual void Update() override;
private:
	LightData _lightData; 
};
}

