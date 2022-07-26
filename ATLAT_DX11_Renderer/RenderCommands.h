#pragma once

#include "DX11Renderer.h"
#include "Mesh.h"
#include "ConstantBuffer.h"
#include "Material.h"

namespace ATLAT::DX11
{

class RenderCommand_Start : public RenderCommand
{
public:
	virtual void Execute() override
	{
		DX11Renderer::GetInstance()->RenderBegin();
	}
};

class RenderCommand_End : public RenderCommand
{
public:
	virtual void Execute() override
	{
		DX11Renderer::GetInstance()->RenderBegin();
	}
};

class RenderCommand_SetConstantBuffer : public RenderCommand
{
public:
	virtual void Execute() override
	{
		ConstantBuffer::GetInstance().PushData(_data, _size, static_cast<uint32>(_type));
	}
	void SetConstantBuffer(CONSTANT_BUFFER_TYPE type, const void* data, uint32 size)
	{
		_type = type;
		_data = data;
		_size = size;
	}

private:
	CONSTANT_BUFFER_TYPE _type = {};
	const void* _data = nullptr;
	uint32 _size = 0;
};

class RenderCommand_DrawMesh : public RenderCommand
{
public:
	virtual void Execute() override
	{
		_material->Update();
		_mesh->Render();
	}

	void SetMesh(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
	{
		_mesh = mesh;
		_material = material;
	}
private:
	std::shared_ptr<Mesh> _mesh = nullptr;
	std::shared_ptr<Material> _material = nullptr;
};
}

