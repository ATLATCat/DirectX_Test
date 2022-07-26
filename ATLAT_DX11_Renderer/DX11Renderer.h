#pragma once

#include "ID3D_Renderer.h"

namespace ATLAT
{

namespace DX11
{
class Shader;
class Mesh;
class Texture;
class ResourceManager;
class SwapChain;
class RenderTarget;
}

class DX11Renderer : public ID3D_Renderer
{
public:
	virtual ~DX11Renderer();

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

	static std::shared_ptr<DX11Renderer> GetInstance() { return std::static_pointer_cast<DX11Renderer>(_instance); }

private:
	void InitDeferred();

	void DeferredBegin();
	void DeferredEnd();

	template <typename T>
	void PushCommand(std::shared_ptr<T> command, uint8 layer);

private:
	WindowInfo _window;

	std::array<std::vector<std::shared_ptr<RenderCommand>>, 5> _renderCommands;

	ComPtr<ID3D11DeviceContext> _deviceContext;
	std::shared_ptr<DX11::SwapChain> _swapChain;


	DX11::ResourceManager* _resourceManager;

	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;

	ComPtr<ID3D11RenderTargetView> _swapChainBufferView;

#pragma region TestCode
	std::shared_ptr<DX11::Shader> _shader;
	std::shared_ptr<DX11::Texture> _texture;
	std::shared_ptr<DX11::Mesh> _deferredMesh;
	std::shared_ptr<DX11::Shader> _deferredDebugShader;

	std::vector<std::shared_ptr<DX11::RenderTarget>> _renderTargets;
	std::vector<ComPtr<ID3D11RenderTargetView>> _renderTargetViews;
	std::vector<ComPtr<ID3D11ShaderResourceView>> _shaderResourceViews;

	ComPtr<ID3D11SamplerState> _samplerState;
#pragma endregion
};

template<typename T>
inline void DX11Renderer::PushCommand(std::shared_ptr<T> command, uint8 layer = 0)
{
	_renderCommands[layer].push_back(command);
}
}
