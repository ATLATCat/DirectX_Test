#include "pch.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Shader.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"

ATLAT::DX12::Material::Material()
{
	_constantBuffer = DX12Renderer::GetInstance()->GetConstantBuffer(CONSTANT_BUFFER_TYPE::Material);
	_tableDescriptorHeap = DX12Renderer::GetInstance()->GetTableDescriptorHeap();
}

void ATLAT::DX12::Material::Init(int size)
{
	_data.resize(size / sizeof(uint32));
}

void ATLAT::DX12::Material::SetData(const void* data, uint32 size, uint8 offset)
{
	::memcpy(&_data[offset], data, size);
}

void ATLAT::DX12::Material::Update()
{
	_constantBuffer->PushData(&_data[0], _data.size() * sizeof(uint32));

	for (uint32 i = 0; i < _textures.size(); i++)
	{
		if (_textures[i] == nullptr)
			continue;

		SRV_REGISTER srv_reg = SRV_REGISTER(static_cast<uint8>(SRV_REGISTER::t0) + i);
		_tableDescriptorHeap->SetSRV(_textures[i]->GetCpuHandle(), srv_reg);
	}

	_shader->Update();
}
