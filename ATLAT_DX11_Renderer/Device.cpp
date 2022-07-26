#include "pch.h"
#include "Device.h"

void ATLAT::DX11::Device::Init(const WindowInfo& window, D3D11_VIEWPORT& viewport)
{
	UINT createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;

	HR(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0,
		createDeviceFlags,
		0, 0,
		D3D11_SDK_VERSION,
		&_device,
		&featureLevel,
		&_immediateContext));

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		assert(false);
	}


	ComPtr<IDXGIDevice> dxgiDevice;
	HR(_device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

	ComPtr<IDXGIAdapter> dxgiAdapter;
	HR(dxgiDevice->GetParent(IID_PPV_ARGS(&dxgiAdapter)));

	HR(dxgiAdapter->GetParent(IID_PPV_ARGS(&_dxgiFactory)));


	_immediateContext->RSSetViewports(1, &viewport);

	HR(_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &_massQuality));
	_Assert(_massQuality > 0);
}

void ATLAT::DX11::Device::CreateDepthStencilView(const WindowInfo& window, UINT massQuality)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = window.width;
	depthStencilDesc.Height = window.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//나중에 어떻게 할지 생각해보자
	if (true)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = massQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;

	HR(_device->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer));
	HR(_device->CreateDepthStencilView(depthStencilBuffer, 0, &_depthStencilView));
}