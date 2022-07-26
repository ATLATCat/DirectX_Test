#include "pch.h"
#include "Mesh.h"
#include "DX12Renderer.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Material.h"
#include "CommandQueue.h"
#include "Device.h"


ATLAT::DX12::Mesh::Mesh()
{
	_commandList = DX12Renderer::GetInstance()->GetCommandQueue()->GetCommandList();
}

void ATLAT::DX12::Mesh::Render() 
{
	_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_commandList->IASetVertexBuffers(0, 1, &_vertexBufferView);
	_commandList->IASetIndexBuffer(&_indexBufferView);

	DX12Renderer::GetInstance()->GetTableDescriptorHeap()->CommitTable();

	_commandList->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}

void ATLAT::DX12::Mesh::CreateVertexBuffer(const void* buffer, uint32 vertexSize, uint32 count)
{
	_Assert(_vertexBuffer == nullptr);
	_vertexCount = count;
	uint32 bufferSize = _vertexCount * vertexSize;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Device> device = DX12Renderer::GetInstance()->GetDevice()->GetDeivce();
	HR(device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer)));

	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	HR(_vertexBuffer->Map(0, &readRange, &vertexDataBuffer));
	::memcpy(vertexDataBuffer, buffer, bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = vertexSize;
	_vertexBufferView.SizeInBytes = bufferSize;
}

void ATLAT::DX12::Mesh::CreateIndexBuffer(const void* buffer, uint32 indexSize, uint32 count)
{
	_Assert(_indexBuffer == nullptr);
	_indexCount = count;
	uint32 bufferSize = _indexCount * indexSize;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Device> device = DX12Renderer::GetInstance()->GetDevice()->GetDeivce();
	HR(device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer)));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	HR(_indexBuffer->Map(0, &readRange, &indexDataBuffer));
	::memcpy(indexDataBuffer, buffer, bufferSize);
	_indexBuffer->Unmap(0, nullptr);

	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	if (indexSize != 4)
		_Assert(false);
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	_indexBufferView.SizeInBytes = bufferSize;
}
