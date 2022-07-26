#include "pch.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "DX12Renderer.h"
#include "Device.h"
#include "CommandQueue.h"

ATLAT::DX12::ConstantBuffer::ConstantBuffer()
{
}

ATLAT::DX12::ConstantBuffer::~ConstantBuffer()
{
	if (_constantBuffer)
	{
		if (_constantBuffer != nullptr)
		{
			_constantBuffer->Unmap(0, nullptr);
		} 
		_constantBuffer = nullptr;
	}
}

void ATLAT::DX12::ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{ 
	_reg = reg;

	//constantBuffer는 256배수로 만들어야함
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	ComPtr<ID3D12Device> device = DX12Renderer::GetInstance()->GetDevice()->GetDeivce();
	CreateBuffer(device);
	CreateView(device);

	_tableDescriptorHeap = DX12Renderer::GetInstance()->GetTableDescriptorHeap();
}

void ATLAT::DX12::ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ATLAT::DX12::ConstantBuffer::PushData(const void* buffer, uint32 size)
{
	_Assert(_currentIndex < _elementCount);
	_Assert(_elementSize == ((size + 255) & ~255));

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);
	/*D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	COMMAND_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);*/

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(_currentIndex);
	_tableDescriptorHeap->SetCBV(cpuHandle, _reg);

	_currentIndex++;
}

void ATLAT::DX12::ConstantBuffer::SetGlobalData(const void* buffer, uint32 size)
{
	_Assert(_elementSize == ((size + 255) & ~255));
	::memcpy(&_mappedBuffer[0], buffer, size);
	DX12Renderer::GetInstance()->GetCommandQueue()->GetCommandList()->SetGraphicsRootConstantBufferView(0, GetGpuVirtualAddress(0));
}

D3D12_GPU_VIRTUAL_ADDRESS ATLAT::DX12::ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS virtualAddress = _constantBuffer->GetGPUVirtualAddress();
	virtualAddress += index * _elementSize;
	return virtualAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ATLAT::DX12::ConstantBuffer::GetCpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}

void ATLAT::DX12::ConstantBuffer::CreateBuffer(ComPtr<ID3D12Device> device)
{
	uint32 buffersize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(buffersize);

	HR(device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_constantBuffer)));
	_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
}

void ATLAT::DX12::ConstantBuffer::CreateView(ComPtr<ID3D12Device> device)
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = _elementCount;
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	HR(device->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap)));

	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	//gpu 마다 IncrementSize가 다를 수 있음
	_handleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < _elementCount; i++)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = _constantBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		cbvDesc.SizeInBytes = _elementSize;

		device->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}
