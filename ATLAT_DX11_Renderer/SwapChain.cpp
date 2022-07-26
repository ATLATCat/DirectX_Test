#include "pch.h"
#include "SwapChain.h"
#include "Device.h"

void ATLAT::DX11::SwapChain::Init(const WindowInfo& window)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	swapChainDesc.BufferDesc.Width = window.width;
	swapChainDesc.BufferDesc.Height = window.height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (false/*_enableMsaa*/)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = Device::GetInstance().GetMASSQuality() - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window.hwnd;
	swapChainDesc.Windowed = window.windowed;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	HR(Device::GetInstance().GetDXGIFactory()->CreateSwapChain(Device::GetInstance().GetDevice().Get(), &swapChainDesc, &_swapChain));
}

void ATLAT::DX11::SwapChain::Present(UINT syncInterval, UINT flags)
{
	_swapChain->Present(syncInterval, flags);
}

void ATLAT::DX11::SwapChain::CreateSwapChain(const WindowInfo& window, ComPtr<IDXGIFactory> factory, UINT massQuality)
{
}