#pragma once

namespace ATLAT
{
	class GameObject;
}

class Scene
{
public:
	void Load(const std::wstring& path);

private:
	std::vector<std::shared_ptr<GameObject>> _gameObjects;
};
