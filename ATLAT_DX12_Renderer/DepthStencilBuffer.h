#pragma once

namespace ATLAT::DX12
{
class DepthStencilBuffer
{
public:
	void Init(const WindowInfo& window, DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT);

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle() { return _handle; }
	DXGI_FORMAT GetFormat() { return _format; }
private:
	ComPtr<ID3D12Resource> _buffer;
	ComPtr<ID3D12DescriptorHeap> _heap;
	D3D12_CPU_DESCRIPTOR_HANDLE _handle = {};
	DXGI_FORMAT _format = {};
};
}

