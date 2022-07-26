#include "pch.h"
#include "ComponentManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component.h"

void ATLAT::ComponentManager::Release()
{
	_awakeComponents.clear();
	_addedComponents.clear();
	_inactiveComponents.clear();
	_componentsToBeDestroy.clear();
	for (auto& components : _componentsList)
	{
		components.clear();
	}

	delete &GetInstance();
}

void ATLAT::ComponentManager::Start()
{
	//이전 프레임에 추가된 컴포넌트 한 번에 추가
	for (auto com : _addedComponents)
	{
		Component::Type type = com->GetType();
		_componentsList[static_cast<uint8>(type)].push_back(com);
	}

	for (auto component : _awakeComponents)
	{
		component->Awake();
	}

	for (auto component : _addedComponents)
	{
		component->Start();
	}

	_awakeComponents.clear();
	_addedComponents.clear();
}

void ATLAT::ComponentManager::Update()
{
	for (auto& components : _componentsList)
	{
		for (auto& com : components)
		{
			com->Update();
		}
	}

	for (auto& components : _componentsList)
	{
		for (auto& com : components)
		{
			com->LateUpdate();
		}
	}
}

void ATLAT::ComponentManager::Clear()
{


	_componentsToBeDestroy.clear();
}

void ATLAT::ComponentManager::AddComponent(std::shared_ptr<Component> component)
{
	_addedComponents.push_back(component);
	_awakeComponents.push_back(component);
}

void ATLAT::ComponentManager::AddComponent(std::vector<std::shared_ptr<Component>>& components, bool isActive)
{
	if (isActive)
	{
		_addedComponents.insert(_addedComponents.end(), components.begin(), components.end());
	}
	else
	{
		_inactiveComponents.insert(_inactiveComponents.end(), components.begin(), components.end());
	}

	_awakeComponents.insert(_awakeComponents.end(), components.begin(), components.end());
}

void ATLAT::ComponentManager::RemoveComponent(std::shared_ptr<Component> component)
{
	_componentsToBeDestroy.push_back(component);
}

void ATLAT::ComponentManager::SetActive(bool value, std::shared_ptr<Component> component)
{
	if (value)
	{
		for (auto iter = _inactiveComponents.begin(); iter != _inactiveComponents.end(); ++iter)
		{
			if ((*iter) == component)
			{
				*iter = *(_inactiveComponents.end() - 1);
				_inactiveComponents.resize(_inactiveComponents.size() - 1);
				_componentsList[static_cast<uint8>(component->GetType())].push_back(component);
				break;
			}
		}

	}
	else
	{
		auto components = _componentsList[static_cast<uint8>(component->GetType())];
		for (auto iter = components.begin(); iter != components.end(); ++iter)
		{
			if ((*iter) == component)
			{
				*iter = *(components.end() - 1);
				components.resize(components.size() - 1);
				break;
			}
		}
		_inactiveComponents.push_back(component);
	}
}

void ATLAT::ComponentManager::SetActive(bool value, std::vector<std::shared_ptr<Component>>& components)
{
	for (auto com : components)
	{
		SetActive(value, com);
	}
}
