#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

void ATLAT::DX11::ResourceManager::Init(const std::wstring path)
{
	_resourcesPath = path;
}

std::shared_ptr<Mesh> ATLAT::DX11::ResourceManager::CreateMesh()
{
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	_meshes.insert(std::make_pair(mesh->GetUID(), mesh));

	return mesh;
}

std::shared_ptr<Material> ATLAT::DX11::ResourceManager::CreateMaterial()
{
	std::shared_ptr<Material> material = std::make_shared<Material>();
	_materials.insert(std::make_pair(material->GetUID(), material));

	return material;
}

std::shared_ptr<Texture> ATLAT::DX11::ResourceManager::GetTexture(UID uid)
{
	auto findIter = _textures.find(uid);
	if (findIter == _textures.end())
	{
		_Assert(false);
		return nullptr;
	}
	return findIter->second;
}

std::shared_ptr<Mesh> ATLAT::DX11::ResourceManager::GetMesh(UID uid)
{
	auto findIter = _meshes.find(uid);
	if (findIter == _meshes.end())
	{
		_Assert(false);
		return nullptr;
	}
	return findIter->second;
}

std::shared_ptr<Material> ATLAT::DX11::ResourceManager::GetMaterial(UID uid)
{
	auto findIter = _materials.find(uid);
	if (findIter == _materials.end())
	{
		_Assert(false);
		return nullptr;
	}
	return findIter->second;
}

std::shared_ptr<Shader> ATLAT::DX11::ResourceManager::GetShader(UID uid)
{
	auto findIter = _shaders.find(uid);
	if (findIter == _shaders.end())
	{
		_Assert(false);
		return nullptr;
	}
	return findIter->second;
}

std::shared_ptr<Texture> ATLAT::DX11::ResourceManager::LoadTexture(const std::wstring& name)
{
	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	texture->LoadTexture(_resourcesPath + name);
	_textures.insert(std::make_pair(texture->GetUID(), texture));

	return texture;
}

std::shared_ptr<Shader> ATLAT::DX11::ResourceManager::LoadShader(const std::wstring& name)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->Init(_resourcesPath + name);
	_shaders.insert(std::make_pair(shader->GetUID(), shader));

	return shader;
}