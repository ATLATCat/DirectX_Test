#pragma once

#include "Singleton.h"

namespace ATLAT
{
class Mesh;
class Texture;
class Material;
class Shader;

class ResourceManager : public Singleton<ResourceManager>
{
public:
	void Init();

	std::shared_ptr<Mesh> CreateMesh();
	std::shared_ptr<Mesh> CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);
	
	std::shared_ptr<Material> CreateMaterial();

	std::shared_ptr<Texture> LoadTexture(const std::wstring& name);
	std::shared_ptr<Shader> LoadShader(const std::wstring& name);
private:
	std::unordered_map<UID, std::shared_ptr<Mesh>> _meshes;
	std::unordered_map<UID, std::shared_ptr<Texture>> _textures;
	std::unordered_map<UID, std::shared_ptr<Material>> _shaders;
	std::shared_ptr<ID3D_Renderer> _renderer;
};
}

