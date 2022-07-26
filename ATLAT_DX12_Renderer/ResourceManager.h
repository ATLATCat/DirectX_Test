#pragma once

#include "Singleton.h"

namespace ATLAT::DX12
{
class Texture;
class Mesh;
class Material;
class Shader;

enum class ResourceType : uint8
{
	Texture,
	Mesh,
	Material,
	Shader,
	End
};

class ResourceManager : public Singleton<ResourceManager>
{
public:
	void Init(const std::wstring path);

	std::shared_ptr<Mesh> CreateMesh();
	std::shared_ptr<Material> CreateMaterial();

	std::shared_ptr<Texture> GetTexture(UID uid);
	std::shared_ptr<Mesh> GetMesh(UID uid);
	std::shared_ptr<Material> GetMaterial(UID uid);
	std::shared_ptr<Shader> GetShader(UID uid);

	const std::unordered_map<UID, std::shared_ptr<Mesh>>& GetMeshes() { return _meshes; }

	void AddMesh(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Texture> LoadTexture(const std::wstring& name);
	std::shared_ptr<Shader> LoadShader(const std::wstring& name);
private:
	std::unordered_map<UID, std::shared_ptr<Texture>> _textures;
	std::unordered_map<UID, std::shared_ptr<Mesh>> _meshes;
	std::unordered_map<UID, std::shared_ptr<Material>> _materials;
	std::unordered_map<UID, std::shared_ptr<Shader>> _shaders;


	std::wstring _resourcesPath;
};
}

