#pragma once

namespace ATLAT::DX12
{

enum
{
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END),
};

struct TransformConstantBuffer
{
	Matrix worldMatrix; 
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg, uint32 size, uint32 count);
	void Clear();
	void PushData(const void* buffer, uint32 size);
	void SetGlobalData(const void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);
private:
	void CreateBuffer(ComPtr<ID3D12Device> device);
	void CreateView(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12Resource> _constantBuffer;
	BYTE* _mappedBuffer = nullptr;
	uint32 _elementSize = 0;
	uint32 _elementCount = 0;
	uint32 _currentIndex = 0;

	ComPtr<ID3D12DescriptorHeap> _cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandleBegin = {};
	uint32 _handleIncrementSize = 0;

	std::shared_ptr<class TableDescriptorHeap> _tableDescriptorHeap;
	CBV_REGISTER _reg = {};
};
}
