#pragma once

namespace ATLAT::DX12
{
class ResourceManager;
class Mesh;
class Material;

class Renderer
{
public:
	void DrawMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
	std::shared_ptr<Mesh> CreateMesh();
	std::shared_ptr<Material> CreateMaterial();

private:
	std::shared_ptr<ResourceManager> _resourceManager;
};
}

