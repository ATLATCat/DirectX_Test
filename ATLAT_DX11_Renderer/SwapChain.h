#pragma once

namespace ATLAT::DX11
{
class Device;

class SwapChain
{
public:
	void Init(const WindowInfo& window);
	void Present(UINT SyncInterval, UINT Flags);

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }

private:
	void CreateSwapChain(const WindowInfo& window, ComPtr<IDXGIFactory> factory, UINT massQuality);

	ComPtr<IDXGISwapChain> _swapChain;
	bool _enableMsaa = true;
};
}

