#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "DepthStencilBuffer.h"
#include "RootSignature.h"

ATLAT::DX12::CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);
}

void ATLAT::DX12::CommandQueue::Init(ComPtr<ID3D12Device> device, std::shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;
	_rootSignature = DX12Renderer::GetInstance()->GetRootSignature()->GetSignature();

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	HR(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_queue)));
	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_allocator)));
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _allocator.Get(), nullptr, IID_PPV_ARGS(&_list)));
	HR(_list->Close());

	HR(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resourceCommandAllocator)));
	HR(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resourceCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&_resourceCommandList)));

	HR(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence)));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

	_tableDescriptorHeap = DX12Renderer::GetInstance()->GetTableDescriptorHeap();
}

void ATLAT::DX12::CommandQueue::WaitSync()
{ 
	_fenceValue++;
	_queue->Signal(_fence.Get(), _fenceValue);
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}

void ATLAT::DX12::CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	_allocator->Reset();
	_list->Reset(_allocator.Get(), nullptr);

	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	_list->SetGraphicsRootSignature(_rootSignature.Get());
	DX12Renderer::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::Transform)->Clear();
	DX12Renderer::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::Material)->Clear();
	_tableDescriptorHeap->Clear();

	ID3D12DescriptorHeap* descHeap = _tableDescriptorHeap->GetDescriptorHeap().Get();
	_list->SetDescriptorHeaps(1, &descHeap);

	_list->ResourceBarrier(1, &barrier);

	_list->RSSetViewports(1, vp);
	_list->RSSetScissorRects(1, rect);

	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_list->ClearRenderTargetView(backBufferView, DirectX::Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = DX12Renderer::GetInstance()->GetDepthStencilBuffer()->GetHandle();
	_list->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);
	_list->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void ATLAT::DX12::CommandQueue::RenderEnd()
{
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT
	);

	_list->ResourceBarrier(1, &barrier);
	_list->Close();

	ID3D12CommandList* commandLists[] = { _list.Get() };
	_queue->ExecuteCommandLists(_countof(commandLists), commandLists);

	_swapChain->Present();

	WaitSync();

	_swapChain->SwapIndex();
}

void ATLAT::DX12::CommandQueue::FlushResourceCommandQueue()
{
	_resourceCommandList->Close();

	ID3D12CommandList* cmdLists[] = { _resourceCommandList.Get() };
	_queue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	WaitSync();

	_resourceCommandAllocator->Reset();
	_resourceCommandList->Reset(_resourceCommandAllocator.Get(), nullptr);
}
