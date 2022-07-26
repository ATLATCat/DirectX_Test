#include "pch.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "RenderCommands.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

void ATLAT::DX12::Renderer::DrawMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{

	//if (mesh != nullptr)
	//{
	//	std::shared_ptr<RenderCommand_DrawMesh> command_draw = std::make_shared<RenderCommand_DrawMesh>();
	//	command_draw->SetMesh(mesh);
	//	DX12Renderer::GetInstance()->PushCommand(command_draw);
	//	return;
	//}

	//std::shared_ptr<RenderCommand_DrawMesh> command = std::make_shared<RenderCommand_DrawMesh>();
	//command->SetMesh(mesh);
	//DX12Renderer::GetInstance()->PushCommand(command);
}

std::shared_ptr<Mesh> ATLAT::DX12::Renderer::CreateMesh()
{
	_Assert(false);
	//std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

	//
	//mesh->SetName(g_uuidCount++);
	//mesh->Init(meshData.vertices, meshData.indices);

	//_resourceManager->AddMesh(mesh);
	return nullptr;
}

std::shared_ptr<Material> ATLAT::DX12::Renderer::CreateMaterial()
{
	_Assert(false);
	//std::shared_ptr<Material> _material = std::make_shared<Material>();

	//std::shared_ptr<Shader> shader;
	//std::shared_ptr<Texture> texture;

	//shader = _resourceManager.GetShader(materialData.shaderName);
	//texture = _resourceManager.GetTexture(materialData.texture.name);
	//_material->SetShader(shader);
	//_material->SetTexture(0, texture);
	return nullptr;
}
