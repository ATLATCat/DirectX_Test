#pragma once

#include "Object.h"

namespace ATLAT::DX11
{
class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh();

	virtual void Init();
	virtual void Render();

	void CreateVertexBuffer(const void* buffer, uint32 vertexSize, uint32 count);
	void CreateIndexBuffer(const void* buffer, uint32 indexSize, uint32 count);

	void SetCounterClockWise(bool value);

	const std::wstring& GetName() { return _name; }
	void SetName(const std::wstring& name) { _name = name; }
protected:
	ComPtr<ID3D11Buffer> _vertexBuffer;
	ComPtr<ID3D11Buffer> _indexBuffer;

	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11RasterizerState> _rasterizerState;

	int _vertexSize;
	uint32 _indicesCount;

	D3D11_RASTERIZER_DESC _rasterizerDesc;

	ComPtr<ID3D11SamplerState> _sampler;

	std::wstring _name;
};
}
