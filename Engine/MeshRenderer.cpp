#include "pch.h"
#include "MeshRenderer.h"
#include "Renderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"

ATLAT::MeshRenderer::MeshRenderer()
{
}

void ATLAT::MeshRenderer::Awake()
{
	_transform = GetTransform();
}

void ATLAT::MeshRenderer::LateUpdate()
{
	_data.world = _transform->GetWorldMatrix();
	_data.worldView = _data.world * Camera::GetMainCamera()->GetViewMatrix();
	_data.worldViewProj = _data.worldView * Camera::GetMainCamera()->GetProjectionMatrix();
	_data.viewPosition = Camera::GetMainCamera()->GetTransform()->GetPosition();

	Renderer::GetInstance().GetRenderer()->SetConstantBuffer(CONSTANT_BUFFER_TYPE::Transform, &_data, sizeof(_data), _gameObject.lock()->GetLayer().GetValue());

	Renderer::GetInstance().GetRenderer()->DrawMesh(_mesh->GetUID(), _material->GetUID(), _gameObject.lock()->GetLayer().GetValue());
}
