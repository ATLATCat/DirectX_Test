#include "pch.h"
#include "GameObject.h"
#include "ComponentManager.h"

void ATLAT::GameObject::AddComponent(std::shared_ptr<Component> component)
{
	ComponentManager::GetInstance().AddComponent(component);
	_components.push_back(component);
	component->_gameObject = shared_from_this();
}

void ATLAT::GameObject::RemoveComponent(std::shared_ptr<Component> component)
{
	_Assert(false);
}
