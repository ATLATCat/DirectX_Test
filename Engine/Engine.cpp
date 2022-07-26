#include "pch.h"

#include "Engine.h"
#include <FileSystem>
#include <fstream>
#include <string>
#include "ComponentManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <strsafe.h>

WindowInfo ATLAT::Engine::_window;

ATLAT::Engine::~Engine()
{
	Release();
}

void ATLAT::Engine::Release()
{
	delete& Time::GetInstance();
	delete& Input::GetInstance();
	delete& ComponentManager::GetInstance();
	delete& ResourceManager::GetInstance();

	delete& Renderer::GetInstance();
}

void ATLAT::Engine::Init(const WindowInfo& info)
{
	_window = info;

	LoadSetting(L"EngineSetting.txt");

	Time::GetInstance().Init();
	Input::GetInstance().Init(info.hwnd);

	Renderer::GetInstance().LoadRenderer(_dllPath);
	_renderer = Renderer::GetInstance().GetRenderer();

	RendererInitData data;
	data.resourcesPath = _resourcesPath;
	data.windowInfo = info;

	_renderer->Init(data);

	_componentManager = &ComponentManager::GetInstance();
	ResourceManager::GetInstance().Init();
}

void ATLAT::Engine::Update()
{
	Time::GetInstance().Update();
	Input::GetInstance().Update();
	ShowDebugTitle();

	_componentManager->Start();
	_componentManager->Update();
}

void ATLAT::Engine::Render()
{
    _renderer->Render();
}

void ATLAT::Engine::OnMouseMove(int buttonState, int x, int y)
{
	if (OnMouseMoveEvent != nullptr)
	{
		OnMouseMoveEvent(buttonState, x, y);
	}
}

void ATLAT::Engine::OnResize()
{
	_renderer->OnResizeWindow();
}

void ATLAT::Engine::ShowDebugTitle()
{
	uint32_t fps = Time::GetInstance().GetFps();
	if (fps == 0) return;

	WCHAR text[100] = L"";
	StringCbPrintf(text, 100, L"%s  FPS : %d ms : %f  ", Renderer::GetInstance().GetName().c_str(), fps, 1000.f / fps);

	SetWindowText(_window.hwnd, text);
}

void ATLAT::Engine::LoadSetting(std::wstring path)
{
	std::ifstream fin(path);

	if (!fin)
	{
		MessageBox(0, L"setting file note found", 0, 0);
		return;
	}

	std::string line;
	std::string properties;
	std::string value;
	while (std::getline(fin, line))
	{
		size_t index;
		if ((index = line.find(":")) != std::string::npos)
		{
			properties = line.substr(0, index - 1);
			value = line.substr(index + 2);
		}

		if (properties == "DLLPath")
		{
			_dllPath.assign(value.begin(), value.end());
		}
		else if (properties == "ResourcesPath")
		{
			_resourcesPath.assign(value.begin(), value.end());
		}
	}
}
