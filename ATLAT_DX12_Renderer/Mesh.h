#pragma once

#include "Object.h"

namespace ATLAT::DX12
{
class Material;

class Mesh : public Object
{
public:
	Mesh();
	virtual ~Mesh() {};

	void Render();

	Matrix GetMatrix() { return _transformMatrix; }
	void SetMatrix(Matrix matrix) { _transformMatrix = matrix; }
	
	void CreateVertexBuffer(const void* buffer, uint32 vertexSize, uint32 count);
	void CreateIndexBuffer(const void* buffer, uint32 indexSize, uint32 count);
private:

	Matrix _transformMatrix;

	ComPtr<ID3D12Resource> _vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW _vertexBufferView = {};
	uint32 _vertexCount = 0;

	ComPtr<ID3D12Resource> _indexBuffer;
	D3D12_INDEX_BUFFER_VIEW _indexBufferView;
	uint32 _indexCount = 0;

	ComPtr<ID3D12GraphicsCommandList> _commandList;
};
}
