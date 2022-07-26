#pragma once

#include "Singleton.h"

namespace ATLAT::DX11 
{
class Device : public Singleton<Device>
{
public:
	void Init(const WindowInfo& window, D3D11_VIEWPORT& viewport);

	ComPtr<ID3D11Device> GetDevice() { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _immediateContext; }
	ComPtr<IDXGIFactory> GetDXGIFactory() { return _dxgiFactory; }

	UINT GetMASSQuality() const { return _massQuality; }
private:
	void CreateDepthStencilView(const WindowInfo& window, UINT massQuality);
	
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _immediateContext;
	ComPtr<ID3D11RenderTargetView> _renderTargetView;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	ComPtr<IDXGIFactory> _dxgiFactory;

	UINT _massQuality;
};
}