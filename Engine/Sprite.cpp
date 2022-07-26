#include "pch.h"
#include "Sprite.h"
#include "Transform.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "GameObject.h"
#include "ResourceManager.h"

ATLAT::Sprite::Sprite()
{
}

void ATLAT::Sprite::Awake()
{
	_transform = GetTransform();

	CreateMesh();
}

void ATLAT::Sprite::LateUpdate()
{
	_data.world = _transform->GetWorldMatrix() * Camera::GetMainCamera()->GetTransform()->GetWorldMatrix();
	_data.worldView = _data.world * Camera::GetMainCamera()->GetViewMatrix();
	_data.worldViewProj = _data.worldView * Camera::GetMainCamera()->GetProjectionMatrix();
	_data.viewPosition = Camera::GetMainCamera()->GetTransform()->GetPosition();

	Renderer::GetInstance().GetRenderer()->SetConstantBuffer(CONSTANT_BUFFER_TYPE::Transform, &_data, sizeof(_data), _gameObject.lock()->GetLayer().GetValue());

	Renderer::GetInstance().GetRenderer()->DrawMesh(_mesh->GetUID(), _material->GetUID(), _gameObject.lock()->GetLayer().GetValue());
}

void ATLAT::Sprite::CreateMesh()
{
	_mesh = ResourceManager::GetInstance().CreateMesh();
	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;
	std::vector<Vertex> vertices(4);

	vertices[0] = Vertex{ Vector3(-w2, -h2, -d2), Vector2(0.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vertices[1] = Vertex{ Vector3(-w2, +h2, -d2), Vector2(0.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vertices[2] = Vertex{ Vector3(+w2, +h2, -d2), Vector2(1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	vertices[3] = Vertex{ Vector3(+w2, -h2, -d2), Vector2(1.0f, 1.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(1.0f, 0.0f, 0.0f) };
	_mesh->SetVertices(vertices);

	std::vector<uint32> indices(6);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	_mesh->SetIndeices(indices);
}
