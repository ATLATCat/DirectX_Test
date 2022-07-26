#include "pch.h"
#include "DepthStencilBuffer.h"
#include "Device.h"

void ATLAT::DX12::DepthStencilBuffer::Init(const WindowInfo& window, DXGI_FORMAT format)
{
	_format = format;
	ComPtr<ID3D12Device> device = DX12Renderer::GetInstance()->GetDevice()->GetDeivce();

	D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(_format, window.width, window.height);
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE clearValue = CD3DX12_CLEAR_VALUE(_format, 1.f, 0);

	HR(device->CreateCommittedResource(&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&_buffer)));

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

	device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	_handle = _heap->GetCPUDescriptorHandleForHeapStart();
	device->CreateDepthStencilView(_buffer.Get(), nullptr, _handle);
}
