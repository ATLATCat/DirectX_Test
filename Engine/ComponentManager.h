#pragma once

#include "Singleton.h"
#include "Component.h"

namespace ATLAT
{
class Component;


class ComponentManager : public Singleton<ComponentManager>
{
public:
	void Release();

	void Start();
	void Update();
	void Clear();	//�� �������� �������� ���� ������Ʈ �ѹ��� ����

	void AddComponent(std::shared_ptr<Component> component);
	void AddComponent(std::vector<std::shared_ptr<Component>>& components, bool isActive = true);
	void RemoveComponent(std::shared_ptr<Component> component);
	
	void SetActive(bool value, std::shared_ptr<Component> component);
	void SetActive(bool value, std::vector<std::shared_ptr<Component>>& components);
private:
	std::array<std::vector<std::shared_ptr<Component>>, static_cast<uint8>(Component::Type::End)> _componentsList;

	std::vector<std::shared_ptr<Component>> _addedComponents;		//���� �߰��Ǵ� com�� �� ���� ��Ƽ� ó���ϱ� ���� �迭
	std::vector<std::shared_ptr<Component>> _componentsToBeDestroy;	//�������� �������� �ѹ��� �����ϱ� ���� ��Ƶδ� �迭
	std::vector<std::shared_ptr<Component>> _inactiveComponents;	//��Ȱ�� components
	std::vector<std::shared_ptr<Component>> _awakeComponents;
};
}

