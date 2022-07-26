#pragma once

namespace ATLAT
{
class Transform;
class GameObject;

class Component
{
public:
	enum class Type : uint8
	{
		Physics,
		Action,
		Camera,
		Randering,
		End
	};
public:
	Component(Type type);
	virtual ~Component() {};
public:
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};

	bool GetIsActive() { return _isActive; }
	void SetIsActive(bool value) { _isActive = value; }

	std::shared_ptr<Transform> GetTransform();

	Type GetType() const { return _type; }

protected:
	std::weak_ptr<GameObject> _gameObject;
	Type _type;

	bool _isActive = false;

	friend GameObject;
};
}