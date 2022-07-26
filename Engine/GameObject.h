#pragma once

#include "LayerMask.h"

namespace ATLAT
{
class Component;
class Transform;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	const std::string& GetName() { return _name; }
	
	template<typename T>
	std::shared_ptr<T> GetComponent();
	void AddComponent(std::shared_ptr<Component> component);
	void RemoveComponent(std::shared_ptr<Component> component);

	LayerMask& GetLayer() { return _layer; }
private:
	std::string _name = "";
	std::vector<std::shared_ptr<Component>> _components = {};
	std::shared_ptr<Transform> _transform = nullptr;

	LayerMask _layer;
};
template<typename T>
inline std::shared_ptr<T> GameObject::GetComponent()
{
	for (auto& com : _components)
	{
		if (typeid(*com).name() == typeid(T).name())
		{
			return std::static_pointer_cast<T>(com);
		}
	}
	return nullptr;
}
}

