#pragma once

namespace ATLAT::DX11
{
class Device;

class RenderTarget
{
public:
	void Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11Resource> resource);
	void Init(UINT width, UINT height, bool isMSAA);

	ComPtr<ID3D11RenderTargetView> GetRenderTargetView() { return _renderTargetView; }
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return _shaderResourceView; }

	static ComPtr<ID3D11DepthStencilView> CreateDepthStencilView(UINT width, UINT height, bool isMSAA);
private:
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView;
};
}

