#include "pch.h"

#include "DX11Renderer.h"
#include "Device.h"
#include "ResourceManager.h"
#include "RenderCommands.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "SwapChain.h"
#include "RenderTarget.h"

//Test code
#include <filesystem>
#include <fstream>
#include <string>
#include "ConstantBuffer.h"
namespace fs = std::filesystem;
//

ATLAT::DX11Renderer::~DX11Renderer()
{
	delete &ConstantBuffer::GetInstance();
	delete _resourceManager;
#ifdef _DEBUG
	ComPtr<ID3D11Debug> Debug;
	Device::GetInstance().GetDevice()->QueryInterface(Debug.GetAddressOf());
	OutputDebugStringA("-------누수 오브젝트 목록입니다--------\r\n");
	Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	OutputDebugStringA("-------------------------------------\r\n");
#endif
	delete &Device::GetInstance();
}

void ATLAT::DX11Renderer::Init(const RendererInitData& initData)
{
	_window = initData.windowInfo;
	 
	_resourceManager = &ResourceManager::GetInstance();
	_resourceManager->Init(initData.resourcesPath);

	D3D11_VIEWPORT viewport = { 0, 0, static_cast<FLOAT>(_window.width), static_cast<FLOAT>(_window.height), 0.0f, 1.0f };

	Device::GetInstance().Init(_window, viewport);
	_deviceContext = Device::GetInstance().GetDeviceContext();

	ComPtr<ID3D11Texture2D> renderTarget;
	_swapChain = std::make_shared<SwapChain>();
	_swapChain->Init(_window);
	_swapChain->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&renderTarget));
	Device::GetInstance().GetDevice()->CreateRenderTargetView(renderTarget.Get(), 0, _renderTargetView.GetAddressOf());
	_depthStencilView = RenderTarget::CreateDepthStencilView(_window.width, _window.height, false);

	ConstantBuffer::GetInstance().Init();

	_renderTargets.emplace_back(std::make_shared<RenderTarget>());
	_renderTargets.emplace_back(std::make_shared<RenderTarget>());
	_renderTargets.emplace_back(std::make_shared<RenderTarget>());

	for (auto renderTarget : _renderTargets)
	{
		renderTarget->Init(_window.width, _window.height, false);
		_renderTargetViews.push_back(renderTarget->GetRenderTargetView());
		_shaderResourceViews.push_back(renderTarget->GetShaderResourceView());
	}

	ComPtr<ID3D11Texture2D> backBuffer;
	_swapChain->GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	Device::GetInstance().GetDevice()->CreateRenderTargetView(backBuffer.Get(), 0, _swapChainBufferView.GetAddressOf());

	InitDeferred();

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 2;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[0] = 0.f;
	samplerDesc.BorderColor[1] = 0.f;
	samplerDesc.BorderColor[2] = 0.f;
	samplerDesc.BorderColor[3] = 0.f;
	samplerDesc.MinLOD = -FLT_MAX;
	samplerDesc.MaxLOD = FLT_MAX;
	Device::GetInstance().GetDevice()->CreateSamplerState(&samplerDesc, _samplerState.GetAddressOf());

	_deviceContext->VSSetSamplers(0, 1, _samplerState.GetAddressOf());
	_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf());

	ResizeWindow(_window.width, _window.height);
}

void ATLAT::DX11Renderer::Render()
{
	RenderBegin();

	for (auto& layers : _renderCommands)
	{
		for (auto& command : layers)
		{
			command->Execute();
		}
	}

	RenderEnd();

	for (auto& layers : _renderCommands)
	{
		layers.clear();
	}
}

void ATLAT::DX11Renderer::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), DirectX::Colors::LightSteelBlue);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ATLAT::DX11Renderer::RenderEnd()
{
	_swapChain->Present(0, 0);
}

void ATLAT::DX11Renderer::ResizeWindow(int32 width, int32 height)
{
	_window.width = width;
	_window.height = height;
	RECT rect = { 0, 0, width, height };
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0);
}

void ATLAT::DX11Renderer::OnResizeWindow()
{
	RECT rect;
	GetClientRect(_window.hwnd, &rect);
	_window.width = rect.right - rect.left;
	_window.height = rect.bottom - rect.top;
}

void ATLAT::DX11Renderer::CreateMesh(UID* out_uid)
{
	*out_uid = _resourceManager->CreateMesh()->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX11Renderer::SetVertices(const void* buffer, uint32 vertexSize, uint32 count, UID uid)
{
	_resourceManager->GetMesh(uid)->CreateVertexBuffer(buffer, vertexSize, count);
}

void ATLAT::DX11Renderer::SetIndices(const void* buffer, uint32 indexSize, uint32 count, UID uid)
{
	_resourceManager->GetMesh(uid)->CreateIndexBuffer(buffer, indexSize, count);
}

void ATLAT::DX11Renderer::CreateMateiral(UID* out_uid, uint32 size)
{
	std::shared_ptr<Material> m = _resourceManager->CreateMaterial();
	m->Init(size);
	*out_uid = m->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX11Renderer::SetMaterialData(const void* buffer, uint32 size, UID uid_material, uint32 offset)
{
	_resourceManager->GetMaterial(uid_material)->SetData(buffer, size, offset);
}

void ATLAT::DX11Renderer::LoadTexture(const std::wstring& path, UID* out_uid)
{
	*out_uid = _resourceManager->LoadTexture(path)->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX11Renderer::SetTexture(UID uid_material, UID uid_texture)
{
	_resourceManager->GetMaterial(uid_material)->SetTexture(0, _resourceManager->GetTexture(uid_texture));
}

void ATLAT::DX11Renderer::LoadShader(const std::wstring& path, UID* out_uid)
{
	*out_uid = _resourceManager->LoadShader(path)->GetUID();
	_Assert(*out_uid != 0);
}

void ATLAT::DX11Renderer::SetShader(UID uid_material, UID uid_shader)
{
	_resourceManager->GetMaterial(uid_material)->SetShader(_resourceManager->GetShader(uid_shader));
}

void ATLAT::DX11Renderer::DrawMesh(UID uid_mesh, UID uid_material, uint8 layer)
{
	ResourceManager& resrouceManager = ResourceManager::GetInstance();

	std::shared_ptr<Mesh> mesh = resrouceManager.GetMesh(uid_mesh);
	std::shared_ptr<Material> material = resrouceManager.GetMaterial(uid_material);

	_Assert(mesh != nullptr);
	_Assert(material != nullptr);

	if (mesh != nullptr)
	{
		std::shared_ptr<RenderCommand_DrawMesh> command = std::make_shared<RenderCommand_DrawMesh>();
		command->SetMesh(mesh, material);
		PushCommand(command, layer);
		return;
	}
}


void ATLAT::DX11Renderer::SetConstantBuffer(CONSTANT_BUFFER_TYPE type, const void* data, uint32 size, uint8 layer)
{
	std::shared_ptr<RenderCommand_SetConstantBuffer> command = std::make_shared<RenderCommand_SetConstantBuffer>();
	command->SetConstantBuffer(type, data, size);
	PushCommand(command, layer);
}

void ATLAT::DX11Renderer::InitDeferred()
{
	_deferredMesh = _resourceManager->CreateMesh();
	_deferredMesh->Init();

	struct Test {
		Vector3 position;
		Vector2 uv;
	};

	float w2 = 0.5f;
	float h2 = 0.5f;
	float d2 = 0.5f;
	std::vector<Test> vertices(4);


	vertices[0] = Test{ Vector3(-w2, -h2, d2), Vector2(0.0f, 1.0f) };
	vertices[1] = Test{ Vector3(-w2, +h2, d2), Vector2(0.0f, 0.0f) };
	vertices[2] = Test{ Vector3(+w2, +h2, d2), Vector2(1.0f, 0.0f) };
	vertices[3] = Test{ Vector3(+w2, -h2, d2), Vector2(1.0f, 1.0f) };

	_deferredMesh->CreateVertexBuffer(&vertices[0], sizeof(Test), static_cast<uint32>(vertices.size()));

	std::vector<uint32> indices(6);

	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;
	_deferredMesh->CreateIndexBuffer(&indices[0], sizeof(uint32), static_cast<uint32>(indices.size()));

	_shader = std::make_shared<DX11::Shader>();
	_shader->InitDeferredShader(_resourceManager->GetResourcePath() + L"Shader\\Deferred.hlsli");
	_deferredDebugShader = std::make_shared<DX11::Shader>();
	_deferredDebugShader->InitDeferredShader(_resourceManager->GetResourcePath() + L"Shader\\DeferredDebug.hlsli");
}

void ATLAT::DX11Renderer::DeferredBegin()
{
	_deviceContext->OMSetRenderTargets(3, _renderTargetViews[0].GetAddressOf(), _depthStencilView.Get());

	_deviceContext->ClearRenderTargetView(_renderTargetViews[0].Get(), DirectX::Colors::White);
	_deviceContext->ClearRenderTargetView(_renderTargetViews[1].Get(), DirectX::Colors::White);
	_deviceContext->ClearRenderTargetView(_renderTargetViews[2].Get(), DirectX::Colors::White);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

struct DeferredVertex {
	Vector3 position;
	Vector2 uv;
};
void ATLAT::DX11Renderer::DeferredEnd()
{
	_deviceContext->OMSetRenderTargets(1, _swapChainBufferView.GetAddressOf(), _depthStencilView.Get());
	_deviceContext->ClearRenderTargetView(_swapChainBufferView.Get(), DirectX::Colors::DarkKhaki);
	_deviceContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//D3D11_RASTERIZER_DESC _rasterizerDesc;
	//ZeroMemory(&_rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	//_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//_rasterizerDesc.CullMode = D3D11_CULL_NONE;
	//_rasterizerDesc.DepthClipEnable = false;
	//_rasterizerDesc.FrontCounterClockwise = false;

	//ComPtr<ID3D11RasterizerState> _rasterizerState;
	//HR(Device::GetInstance().GetDevice()->CreateRasterizerState(&_rasterizerDesc, &_rasterizerState));

	//_deviceContext->RSSetState(_rasterizerState.Get());

	_deviceContext->VSSetShader(_deferredDebugShader->GetVertexShader().Get(), nullptr, 0);
	_deviceContext->PSSetShader(_deferredDebugShader->GetPixelShader().Get(), nullptr, 0);
	_deviceContext->IASetInputLayout(_deferredDebugShader->GetInputLayout().Get());

	//임시 테스트 코드
	float w2 = 1.f;
	float h2 = 1.f;
	float d2 = 0.5f;
	std::vector<DeferredVertex> vertices(4);

	vertices[0] = DeferredVertex{ Vector3(-w2, -h2, d2), Vector2(0.0f, 1.0f) };
	vertices[1] = DeferredVertex{ Vector3(-w2, 0, d2), Vector2(0.0f, 0.0f) };
	vertices[2] = DeferredVertex{ Vector3(0, 0, d2), Vector2(1.0f, 0.0f) };
	vertices[3] = DeferredVertex{ Vector3(0, -h2, d2), Vector2(1.0f, 1.0f) };

	_deferredMesh->CreateVertexBuffer(&vertices[0], sizeof(DeferredVertex), static_cast<uint32>(vertices.size()));

	_deviceContext->PSSetShaderResources(0, 1, _shaderResourceViews[0].GetAddressOf());

	_deferredMesh->Render();

	vertices[0] = DeferredVertex{ Vector3(0, -h2, d2), Vector2(0.0f, 1.0f) };
	vertices[1] = DeferredVertex{ Vector3(0, 0, d2), Vector2(0.0f, 0.0f) };
	vertices[2] = DeferredVertex{ Vector3(w2, 0, d2), Vector2(1.0f, 0.0f) };
	vertices[3] = DeferredVertex{ Vector3(w2, -h2, d2), Vector2(1.0f, 1.0f) };

	_deferredMesh->CreateVertexBuffer(&vertices[0], sizeof(DeferredVertex), static_cast<uint32>(vertices.size()));
	_deviceContext->PSSetShaderResources(0, 1, _shaderResourceViews[1].GetAddressOf());
	_deferredMesh->Render();

	vertices[0] = DeferredVertex{ Vector3(0, 0, d2), Vector2(0.0f, 1.0f) };
	vertices[1] = DeferredVertex{ Vector3(0, h2, d2), Vector2(0.0f, 0.0f) };
	vertices[2] = DeferredVertex{ Vector3(w2, h2, d2), Vector2(1.0f, 0.0f) };
	vertices[3] = DeferredVertex{ Vector3(w2, 0, d2), Vector2(1.0f, 1.0f) };

	_deferredMesh->CreateVertexBuffer(&vertices[0], sizeof(DeferredVertex), static_cast<uint32>(vertices.size()));
	_deviceContext->PSSetShaderResources(0, 1, _shaderResourceViews[2].GetAddressOf());
	_deferredMesh->Render();


	_deviceContext->VSSetShader(_shader->GetVertexShader().Get(), nullptr, 0);
	_deviceContext->PSSetShader(_shader->GetPixelShader().Get(), nullptr, 0);
	_deviceContext->IASetInputLayout(_shader->GetInputLayout().Get());

	vertices[0] = DeferredVertex{ Vector3(-w2, 0, d2), Vector2(0.0f, 1.0f) };
	vertices[1] = DeferredVertex{ Vector3(-w2, h2, d2), Vector2(0.0f, 0.0f) };
	vertices[2] = DeferredVertex{ Vector3(0, h2, d2), Vector2(1.0f, 0.0f) };
	vertices[3] = DeferredVertex{ Vector3(0, 0, d2), Vector2(1.0f, 1.0f) };

	_deferredMesh->CreateVertexBuffer(&vertices[0], sizeof(DeferredVertex), static_cast<uint32>(vertices.size()));
	_deviceContext->PSSetShaderResources(0, 3, _shaderResourceViews[0].GetAddressOf());
	_deferredMesh->Render();

	_swapChain->Present(0, 0);

	ID3D11ShaderResourceView* const pSRV[3] = { NULL };
	_deviceContext->PSSetShaderResources(0, 3, pSRV);
}
