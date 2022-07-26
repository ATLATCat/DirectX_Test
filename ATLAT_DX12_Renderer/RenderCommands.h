#pragma once

#include "DX12Renderer.h"
#include "Mesh.h"
#include "Material.h"
#include "ConstantBuffer.h"

namespace ATLAT::DX12
{

class RenderCommand_Start : public RenderCommand
{
public:
	virtual void Execute() override
	{
		DX12Renderer::GetInstance()->RenderBegin();
	}
};

class RenderCommand_End : public RenderCommand
{
public:
	virtual void Execute() override
	{
		DX12Renderer::GetInstance()->RenderBegin();
	}
};

class RenderCommand_SetConstantBuffer : public RenderCommand
{
public:
	virtual void Execute() override
	{
		if (_type == CONSTANT_BUFFER_TYPE::Global)
		{
			DX12Renderer::GetInstance()->GetConstantBuffer(_type)->SetGlobalData(_data, _size);
		}
		else
		{
			DX12Renderer::GetInstance()->GetConstantBuffer(_type)->PushData(_data, _size);
		}
	}
	void SetConstantBuffer(CONSTANT_BUFFER_TYPE type, const void* data, uint32 size)
	{
		_type = type;
		_data = data;
		_size = size;
	}

private:
	CONSTANT_BUFFER_TYPE _type;
	const void* _data;
	uint32 _size;
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
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;
};
}

