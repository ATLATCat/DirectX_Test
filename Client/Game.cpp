#include "pch.h"
#include "Game.h"
#include "Scene.h"

Game::~Game()
{
	delete _engine;
}

void Game::Init(const WindowInfo& info)
{
	_engine = &ATLAT::Engine::GetInstance();

	_engine->Init(info);
	
	_scene = std::make_shared<Scene>();
	_scene->Load(L"");
}

void Game::Update()
{
	_engine->Update();
}

void Game::Render()
{
	_engine->Render();
}

void Game::OnSize()
{
	if(_engine != nullptr)
		_engine->OnResize();
}

void Game::OnMouseMove(int buttonState, int x, int y)
{
	_engine->OnMouseMove(buttonState, x, y);
}
