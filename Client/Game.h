#pragma once

#include "Engine.h"

class Scene;

class Game
{

public:
	~Game();

	void Init(const WindowInfo& info);
	void Update();
	void Render();

	void OnSize();

	void OnMouseMove(int buttonState, int x, int y);

private:
	ATLAT::Engine* _engine;
	
	std::wstring _dllPath;

	std::shared_ptr<Scene> _scene;
};

 