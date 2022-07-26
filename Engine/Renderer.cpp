#include "pch.h"
#include "Renderer.h"

ATLAT::Renderer::~Renderer()
{
	_renderer = nullptr;
	_rendererReleaseFunction();
}

void ATLAT::Renderer::LoadRenderer(std::wstring& dllPath)
{
	std::wstring path = dllPath;
	HMODULE hDll = ::LoadLibrary(path.c_str());
	_Assert(hDll != nullptr);
	GetRenderFunction(*func)() = (GetRenderFunction(*)())::GetProcAddress(hDll, "GetRenderer");
	_renderer = func()();

	auto releaseFunction = (GetReleaseFunction(*)())::GetProcAddress(hDll, "GetRelease");
	_rendererReleaseFunction = releaseFunction();

	name = dllPath;
}
