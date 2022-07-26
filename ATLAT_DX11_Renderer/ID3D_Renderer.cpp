#include "pch.h"
#include "ID3D_Renderer.h"
#include "DX11Renderer.h"

std::shared_ptr<ATLAT::ID3D_Renderer> ATLAT::ID3D_Renderer::_instance = nullptr;

std::shared_ptr<ATLAT::ID3D_Renderer> ATLAT::ID3D_Renderer::GetRenderer()
{
	if (_instance == nullptr)
		_instance = std::make_shared<ATLAT::DX11Renderer>();

	return _instance;
}

void ATLAT::ID3D_Renderer::Release()
{
	_instance = nullptr;
}

GetRenderFunction GetRenderer()
{
	return &ATLAT::DX11Renderer::GetRenderer;
}

GetReleaseFunction GetRelease()
{
	return &ATLAT::DX11Renderer::Release;
}
