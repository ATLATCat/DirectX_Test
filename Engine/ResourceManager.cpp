#include "pch.h"
#include "ResourceManager.h"
#include "Mesh.h"
#include "Renderer.h"
#include "ID3D_Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"

void ATLAT::ResourceManager::Init()
{
	_renderer = Renderer::GetInstance().GetRenderer();
}

std::shared_ptr<ATLAT::Mesh> ATLAT::ResourceManager::CreateMesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	UID uid;
	_renderer->CreateMesh(&uid);
	mesh->SetUID(uid);

	return mesh;
}

std::shared_ptr<ATLAT::Mesh> ATLAT::ResourceManager::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices)
{
	_Assert(false);
	return std::shared_ptr<Mesh>();
}

std::shared_ptr<ATLAT::Material> ATLAT::ResourceManager::CreateMaterial()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();

	UID uid;
	_renderer->CreateMateiral(&uid, sizeof(MaterialData));
	material->SetUID(uid);

	return material;
}

std::shared_ptr<ATLAT::Texture> ATLAT::ResourceManager::LoadTexture(const std::wstring& name)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();

	UID uid;
	_renderer->LoadTexture(name, &uid);
	texture->SetUID(uid);

	return texture;
}

std::shared_ptr<ATLAT::Shader> ATLAT::ResourceManager::LoadShader(const std::wstring& name)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

	UID uid;
	_renderer->LoadShader(name, &uid);
	shader->SetUID(uid);

	return shader;
}
