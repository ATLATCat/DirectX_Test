#pragma once

#include "Singleton.h"

namespace ATLAT
{
class Renderer : public Singleton<Renderer>
{
public:
	~Renderer();

	void LoadRenderer(std::wstring& dllPath);

	std::wstring& GetName() { return name; }
	std::shared_ptr<ID3D_Renderer> GetRenderer() { return _renderer; }
private:

	std::shared_ptr<ID3D_Renderer> _renderer;
	std::wstring name;

	GetReleaseFunction _rendererReleaseFunction;
};
}

