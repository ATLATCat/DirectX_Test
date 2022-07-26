#pragma once

namespace ATLAT::DX12
{
class SwapChain;
class DescriptorHeap;

class CommandQueue
{
public:
	~CommandQueue();

	void Init(ComPtr<ID3D12Device> device, std::shared_ptr<SwapChain> swapChain);
	void WaitSync();

	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCommandQueue() { return _queue; }
	ComPtr<ID3D12GraphicsCommandList> GetCommandList() { return _list; }
	ComPtr<ID3D12GraphicsCommandList> GetRsourceCommandList() { return _resourceCommandList; }
private:
	ComPtr<ID3D12CommandQueue> _queue;
	ComPtr<ID3D12CommandAllocator> _allocator;
	ComPtr<ID3D12GraphicsCommandList> _list;

	ComPtr<ID3D12CommandAllocator> _resourceCommandAllocator;
	ComPtr<ID3D12GraphicsCommandList> _resourceCommandList;

	ComPtr<ID3D12Fence> _fence;
	uint32 _fenceValue = 0; 
	HANDLE _fenceEvent = INVALID_HANDLE_VALUE;

	std::shared_ptr<SwapChain> _swapChain;
	
	ComPtr<ID3D12RootSignature> _rootSignature;
	std::shared_ptr<class ConstantBuffer> _constantBuffer;
	std::shared_ptr<class TableDescriptorHeap> _tableDescriptorHeap;
};
}

