#pragma once

#include "Singleton.h"

namespace ATLAT
{
class Engine : public Singleton<Engine>
{
public:
	~Engine();

	void Init(const WindowInfo& info);
	void Update();
	void Render();

	void OnMouseMove(int buttonState, int x, int y);

	void OnResize();

	static const WindowInfo& GetWindowInfo() { return _window; }

	std::function<void(int, int, int)> OnMouseMoveEvent;
private:
	void Release();
	void ShowDebugTitle();

	void LoadSetting(std::wstring path);

private:
	static WindowInfo _window;

	std::shared_ptr<ID3D_Renderer> _renderer;

	std::wstring _dllPath;
	std::wstring _resourcesPath;
	class ComponentManager* _componentManager;
};
}

