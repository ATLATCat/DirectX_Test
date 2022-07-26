#pragma once

#include "Singleton.h"

namespace ATLAT::DX11
{
class ConstantBuffer : public Singleton<ConstantBuffer>
{
public:
	void Init();

	void PushData(const void* data, uint32 size, uint32 slot);
private:
	void CreateBuffer();
	std::array<ComPtr<ID3D11Buffer>, static_cast<uint8>(CONSTANT_BUFFER_TYPE::END)> _constantBuffers;
	ComPtr<ID3D11DeviceContext> _deviceContext;
};
}

