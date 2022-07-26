#include "pch.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Renderer.h"

void ATLAT::Mesh::SetVertices(const std::vector<Vertex>& verteices)
{
	Renderer::GetInstance().GetRenderer()->SetVertices(&verteices[0], sizeof(Vertex), static_cast<uint32>(verteices.size()), GetUID());
}

void ATLAT::Mesh::SetIndeices(const std::vector<uint32>& indices)
{
	Renderer::GetInstance().GetRenderer()->SetIndices(&indices[0], sizeof(uint32), static_cast<uint32>(indices.size()), GetUID());
}
