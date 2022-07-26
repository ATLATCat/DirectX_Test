#include "pch.h"
#include "Material.h"
#include "Device.h"
#include "Shader.h"
#include "Texture.h"
#include "ConstantBuffer.h"

ATLAT::DX11::Material::Material()
{
	_deviceContext = Device::GetInstance().GetDeviceContext();
}

void ATLAT::DX11::Material::Init(uint32 size)
{
	_data.resize(size / sizeof(uint32));
}

void ATLAT::DX11::Material::Update()
{
	_deviceContext->VSSetShader(_shader->GetVertexShader().Get(), nullptr, 0);
	_deviceContext->PSSetShader(_shader->GetPixelShader().Get(), nullptr, 0);
	_deviceContext->IASetInputLayout(_shader->GetInputLayout().Get());

	for (uint32 i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		_deviceContext->PSSetShaderResources(0, 1, _textures[i]->GetShaderResourceView().GetAddressOf());
	}
	ConstantBuffer::GetInstance().PushData(&_data[0], _data.size() * sizeof(_data[0]), 2);
}

void ATLAT::DX11::Material::SetData(const void* data, uint32 size, uint8 offset)
{
	::memcpy(&_data[offset], data, size);
}