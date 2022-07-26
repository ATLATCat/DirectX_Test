#include "pch.h"
#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

ATLAT::Component::Component(Type type)
	: _type(type)
{
}

std::shared_ptr<ATLAT::Transform> ATLAT::Component::GetTransform()
{
	return _gameObject.lock()->GetComponent<Transform>();
}
