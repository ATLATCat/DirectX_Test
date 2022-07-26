#include "pch.h"
#include "Light.h"
#include "Renderer.h"

ATLAT::Light::Light()
	: Component(Type::Action)
{
}

ATLAT::Light::~Light()
{
}

void ATLAT::Light::Awake()
{
	_lightData.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	_lightData.diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	_lightData.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	_lightData.direction = { 0.57735f, -0.57735f, 0.57735f };
	_lightData.position = { 0, 0, 0, 1 };
}

void ATLAT::Light::Update()
{
	Renderer::GetInstance().GetRenderer()->SetConstantBuffer(CONSTANT_BUFFER_TYPE::Global, &_lightData, sizeof(_lightData), 0);
}
