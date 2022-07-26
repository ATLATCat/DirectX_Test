#include "pch.h"
#include "ID3D_Renderer.h"
#include "DX12Renderer.h"

std::shared_ptr<ATLAT::ID3D_Renderer> ATLAT::ID3D_Renderer::_instance = nullptr;

std::shared_ptr<ATLAT::ID3D_Renderer> ATLAT::ID3D_Renderer::GetRenderer()
{
	if (_instance == nullptr)
		_instance = std::make_shared<ATLAT::DX12Renderer>();

	return _instance;
}

void ATLAT::ID3D_Renderer::Release()
{
	_instance = nullptr;
}

GetRenderFunction GetRenderer()
{
	return &ATLAT::DX12Renderer::GetRenderer;
}

GetReleaseFunction GetRelease()
{
	return &ATLAT::DX12Renderer::Release;
}
