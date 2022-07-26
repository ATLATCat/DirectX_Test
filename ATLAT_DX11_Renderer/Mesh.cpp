#include "pch.h"
#include "Mesh.h"
#include "Device.h"
#include "Shader.h"
#include "Texture.h"
#include "Device.h"

ATLAT::DX11::Mesh::Mesh()
{
	_device = Device::GetInstance().GetDevice();
	_deviceContext = Device::GetInstance().GetDeviceContext();
}

ATLAT::DX11::Mesh::~Mesh()
{
}

void ATLAT::DX11::Mesh::Init()
{
	ZeroMemory(&_rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	_rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	_rasterizerDesc.CullMode = D3D11_CULL_BACK;
	_rasterizerDesc.DepthClipEnable = true;
	_rasterizerDesc.FrontCounterClockwise = false;

	HR(_device->CreateRasterizerState(&_rasterizerDesc, &_rasterizerState));

	_deviceContext->RSSetState(_rasterizerState.Get());
}

void ATLAT::DX11::Mesh::Render()
{
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	uint32 stride = _vertexSize;
	uint32 offset = 0;

	_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->RSSetState(_rasterizerState.Get());

	_deviceContext->DrawIndexed(_indicesCount, 0, 0);
}

void ATLAT::DX11::Mesh::CreateVertexBuffer(const void* buffer, uint32 vertexSize, uint32 count)
{
	//_Assert(_vertexBuffer == nullptr);
	_vertexBuffer = nullptr;
	_vertexSize = vertexSize;
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = _vertexSize * count; 
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = buffer;

	HR(_device->CreateBuffer(&vbd, &vinitData, _vertexBuffer.GetAddressOf()));
}

void ATLAT::DX11::Mesh::CreateIndexBuffer(const void* buffer, uint32 indexSize, uint32 count)
{
	_Assert(_indexBuffer == nullptr);
	_indicesCount = count;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = indexSize * count;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = buffer;

	HR(_device->CreateBuffer(&ibd, &iinitData, _indexBuffer.GetAddressOf()));
}

void ATLAT::DX11::Mesh::SetCounterClockWise(bool value)
{
	_rasterizerDesc.FrontCounterClockwise = value;
}
