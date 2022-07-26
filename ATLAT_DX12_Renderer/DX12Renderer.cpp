#include "pch.h"

#include "DX12Renderer.h"
#include "TableDescriptorHeap.h"
#include "DepthStencilBuffer.h"
#include "Material.h"

#include "RootSignature.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Device.h"
#include "ConstantBuffer.h"
#include "ResourceManager.h"

#include "RenderCommands.h"

//Test code
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include <filesystem>
#include <fstream>
#include <string>
namespace fs = std::filesystem;
//

ATLAT::DX12Renderer::~DX12Renderer()
{
	delete _resourceManager;
}

void ATLAT::DX12Renderer::Init(const RendererInitData& initData)
{
	_windowInfo = initData.windowInfo;

	_viewport = { 0, 0, static_cast<FLOAT>(_windowInfo.width), static_cast<FLOAT>(_windowInfo.height), 0.0f, 1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, _windowInfo.width, _windowInfo.height);

	_resourceManager = &ResourceManager::GetInstance();
	_resourceManager->Init(initData.resourcesPath);

	_device = std::make_shared<Device>();
	_commandQueue = std::make_shared<CommandQueue>();
	_swapChain = std::make_shared<SwapChain>();
	_rootSignature = std::make_shared<RootSignature>();
	_tableDescriptorHeap = std::make_shared<TableDescriptorHeap>();
	_depthStencilBuffer = std::make_shared<DepthStencilBuffer>();

	_device->Init();
	_rootSignature->Init(_device->GetDeivce());
	_commandQueue->Init(_device->GetDeivce(), _swapChain);
	_swapChain->Init(_windowInfo, _device->GetDeivce(), _device->GetDXGIFactory(), _commandQueue->GetCommandQueue());
	_tableDescriptorHeap->Init(256);
	_depthStencilBuffer->Init(_windowInfo);

	CreateConstantBuffer(CBV_REGISTER::b0, 256, 1);
	CreateConstantBuffer(CBV_REGISTER::b1, sizeof(TransformConstantBuffer), 256);
	CreateConstantBuffer(CBV_REGISTER::b2, sizeof(256), 256);
}

void ATLAT::DX12Renderer::Render()
{
	RenderBegin();

	for (auto command : _renderCommand)
	{
		command->Execute();
	}

	RenderEnd();

	_renderCommand.clear();
}

void ATLAT::DX12Renderer::RenderBegin()
{
	_commandQueue->RenderBegin(&_viewport, &_scissorRect);
}

void ATLAT::DX12Renderer::RenderEnd()
{
	_commandQueue->RenderEnd();
}

void ATLAT::DX12Renderer::ResizeWindow(int32 width, int32 height)
{
	_windowInfo.width = width;
	_windowInfo.height = height;
	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_windowInfo.hwnd, 0, 100, 100, width, height, 0);

	_depthStencilBuffer->Init(_windowInfo);
}

void ATLAT::DX12Renderer::OnResizeWindow()
{
	RECT rect;
	GetClientRect(_windowInfo.hwnd, &rect);
	_windowInfo.width = rect.right - rect.left;
	_windowInfo.height = rect.bottom - rect.top;
}

void ATLAT::DX12Renderer::CreateMesh(UID* out_uid)
{
	*out_uid = _resourceManager->CreateMesh()->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX12Renderer::SetVertices(const void* buffer, uint32 vertexSize, uint32 count, UID uid)
{
	_resourceManager->GetMesh(uid)->CreateVertexBuffer(buffer, vertexSize, count);
}

void ATLAT::DX12Renderer::SetIndices(const void* buffer, uint32 indexSize, uint32 count, UID uid)
{
	_resourceManager->GetMesh(uid)->CreateIndexBuffer(buffer, indexSize, count);
}

void ATLAT::DX12Renderer::CreateMateiral(UID* out_uid, uint32 size)
{
	std::shared_ptr<Material> m = _resourceManager->CreateMaterial();
	m->Init(size);
	*out_uid = m->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX12Renderer::SetMaterialData(const void* buffer, uint32 size, UID uid_material, uint32 offset)
{
	_resourceManager->GetMaterial(uid_material)->SetData(buffer, size, offset);
}

void ATLAT::DX12Renderer::LoadTexture(const std::wstring& path, UID* out_uid)
{
	*out_uid = _resourceManager->LoadTexture(path)->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX12Renderer::SetTexture(UID uid_material, UID uid_texture)
{
	_resourceManager->GetMaterial(uid_material)->SetTexture(0, _resourceManager->GetTexture(uid_texture));
}

void ATLAT::DX12Renderer::LoadShader(const std::wstring& path, UID* out_uid)
{
	*out_uid = _resourceManager->LoadShader(path)->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX12Renderer::SetShader(UID uid_material, UID uid_shader)
{
	_resourceManager->GetMaterial(uid_material)->SetShader(_resourceManager->GetShader(uid_shader));
}

void ATLAT::DX12Renderer::DrawMesh(UID uid_mesh, UID uid_material, uint8 layer)
{
	ResourceManager& resrouceManager = ResourceManager::GetInstance();

	std::shared_ptr<Mesh> mesh = resrouceManager.GetMesh(uid_mesh);
	std::shared_ptr<Material> material = resrouceManager.GetMaterial(uid_material);

	_Assert(mesh != nullptr);

	if (mesh != nullptr)
	{
		std::shared_ptr<RenderCommand_DrawMesh> command_draw = std::make_shared<RenderCommand_DrawMesh>();
		command_draw->SetMesh(mesh, material);
		PushCommand(command_draw);

		return;
	}
}

void ATLAT::DX12Renderer::SetConstantBuffer(CONSTANT_BUFFER_TYPE type, const void* data, uint32 size, uint8 layer)
{
	std::shared_ptr<RenderCommand_SetConstantBuffer> command = std::make_shared<RenderCommand_SetConstantBuffer>();
	command->SetConstantBuffer(type, data, size);
	PushCommand(command);
}

void ATLAT::DX12Renderer::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count)
{
	std::shared_ptr<ConstantBuffer> buffer = std::make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}
