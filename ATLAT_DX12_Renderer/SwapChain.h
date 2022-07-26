#pragma once

namespace ATLAT::DX12
{
class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgiFactory, ComPtr<ID3D12CommandQueue> commandQueue);
	void Present();
	void SwapIndex();


	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; }
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return _rtvBuffer[index]; }

	ComPtr<ID3D12Resource> GetBackRTVBuffer() { return _rtvBuffer[_backBufferIndex]; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return _rtvHandle[_backBufferIndex]; }
private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgiFactory, ComPtr<ID3D12CommandQueue> commandQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);


	ComPtr<IDXGISwapChain> _swapChain;
	ComPtr<ID3D12Resource> _rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];
	uint32 _backBufferIndex = 0;

	ComPtr<ID3D12DescriptorHeap> _rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _rtvHandle[SWAP_CHAIN_BUFFER_COUNT];
};
}