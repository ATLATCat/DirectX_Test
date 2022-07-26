#include "pch.h"
#include "ConstantBuffer.h"
#include "Device.h"

void ATLAT::DX11::ConstantBuffer::Init()
{
	_deviceContext = Device::GetInstance().GetDeviceContext();
	CreateBuffer();
}

void ATLAT::DX11::ConstantBuffer::PushData(const void* data, uint32 size, uint32 slot)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(_deviceContext->Map(_constantBuffers[slot].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, size, data, size);
	_deviceContext->Unmap(_constantBuffers[slot].Get(), 0);
	_deviceContext->VSSetConstantBuffers(slot, 1, _constantBuffers[slot].GetAddressOf());
	_deviceContext->PSSetConstantBuffers(slot, 1, _constantBuffers[slot].GetAddressOf());
}

void ATLAT::DX11::ConstantBuffer::CreateBuffer()
{
	// constant buffer 임시 코드
	D3D11_BUFFER_DESC constantBufferDesc;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.ByteWidth = 256;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDesc.MiscFlags = 0;
	constantBufferDesc.StructureByteStride = 0;

	for (int i = 0; i < static_cast<uint8>(CONSTANT_BUFFER_TYPE::END); ++i)
	{
		HR(Device::GetInstance().GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, _constantBuffers[i].GetAddressOf()));
	}

}
