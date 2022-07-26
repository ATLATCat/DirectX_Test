#pragma once


#include "ID3D_Renderer.h"
#include "Singleton.h"

namespace ATLAT
{
namespace DX12
{
class Device;
class CommandQueue;
class SwapChain;
class RootSignature;
class ConstantBuffer;
class TableDescriptorHeap;
class DepthStencilBuffer;
class ResourceManager;

//testCode
class Mesh;
}

class DX12Renderer : public ID3D_Renderer
{
public:
	virtual ~DX12Renderer();

	//ID3D_Renderer 상속 함수
	virtual void Init(const RendererInitData& initData) override;
	virtual void Render() override;
	virtual void RenderBegin() override;
	virtual void RenderEnd() override;

	virtual void ResizeWindow(int32 width, int32 height) override;
	virtual void OnResizeWindow() override;

	virtual void CreateMesh(UID* out_uid) override;
	virtual void SetVertices(const void* buffer, uint32 vertexSize, uint32 count, UID uid) override;
	virtual void SetIndices(const void* buffer, uint32 indexSize, uint32 count, UID uid) override;

	virtual void CreateMateiral(UID* out_uid, uint32 size) override;
	virtual void SetMaterialData(const void* buffer, uint32 size, UID uid_material, uint32 offset = 0) override;

	virtual void LoadTexture(const std::wstring& path, UID* out_uid) override;
	virtual void SetTexture(UID uid_material, UID uid_texture) override;

	virtual void LoadShader(const std::wstring& path, UID* out_uid) override;
	virtual void SetShader(UID uid_material, UID uid_shader) override;

	virtual void DrawMesh(UID uid_mesh, UID uid_material, uint8 layer) override;

	virtual void SetConstantBuffer(CONSTANT_BUFFER_TYPE type, const void* data, uint32 size, uint8 layer) override;

public:
	static std::shared_ptr<DX12Renderer> GetInstance() { return std::static_pointer_cast<DX12Renderer>(_instance); }
	
	std::shared_ptr<DX12::Device> GetDevice() { return _device; }
	std::shared_ptr<DX12::CommandQueue> GetCommandQueue() { return _commandQueue; }
	std::shared_ptr<DX12::SwapChain> GetSwapChain() { return _swapChain; }
	std::shared_ptr<DX12::RootSignature> GetRootSignature() { return _rootSignature; }
	std::shared_ptr<DX12::TableDescriptorHeap> GetTableDescriptorHeap() { return _tableDescriptorHeap; }
	std::shared_ptr<DX12::DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer; }
	std::shared_ptr<DX12::ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }

	template <typename T>
	void PushCommand(std::shared_ptr<T> command);
private:
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);
	

	WindowInfo _windowInfo;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	std::shared_ptr<DX12::Device> _device;
	std::shared_ptr<DX12::CommandQueue> _commandQueue;
	std::shared_ptr<DX12::SwapChain> _swapChain;
	std::shared_ptr<DX12::RootSignature> _rootSignature;
	std::shared_ptr<DX12::TableDescriptorHeap> _tableDescriptorHeap;
	std::shared_ptr<DX12::DepthStencilBuffer> _depthStencilBuffer;

	std::vector<std::shared_ptr<DX12::ConstantBuffer>> _constantBuffers;

	std::vector<std::shared_ptr<DX12::Mesh>> _renderMeshes;

	std::vector<std::shared_ptr<RenderCommand>> _renderCommand;

	DX12::ResourceManager* _resourceManager;

	//Test Code
	std::wstring _resourcePath;
};

template<typename T>
inline void DX12Renderer::PushCommand(std::shared_ptr<T> command)
{
	_renderCommand.push_back(command);
}
}
