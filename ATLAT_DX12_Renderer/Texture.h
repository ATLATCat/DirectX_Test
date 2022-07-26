#pragma once

#include "Object.h"

namespace ATLAT::DX12
{
class Texture : public Object
{
public:
	virtual ~Texture() {}

	void Init(const std::wstring& path);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle() { return _srvHandle; }

private:
	void CreateTexture(const std::wstring& path);
	void CreateView();

private:
	std::wstring _name;

	DirectX::ScratchImage _image;
	ComPtr<ID3D12Resource> _tex2d;
	ComPtr<ID3D12DescriptorHeap> _srvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _srvHandle;

	ComPtr<ID3D12Device> _device;
};
}

