#pragma once

#include "Component.h"

namespace ATLAT
{
class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	Vector3 GetLocalPosition();

	Vector3 GetPosition();

	Matrix GetLocalMatrix();
	Matrix GetWorldMatrix();

	Vector3 GetRight();
	Vector3 GetForward();
	Vector3 GetUp();

	//�̵�
	void TransformVector(Vector3 vector);
	
	//ȸ��
	void RotateAxis(Vector3 axis, float angle);

private:
	Vector3 _localPosition = {};
	Vector3 _localScale = {};

	Vector3 _forward = Vector3::Forward;	//simpleMath�� ������ ��ǥ�踦 ��....
	Vector3 _right = Vector3::Right;
	Vector3 _up = Vector3::Up;

	Matrix _localMatrix = {};
	Matrix _worldMatrix = {};

	std::weak_ptr<Transform> _parent = {};
	std::vector <std::shared_ptr<Transform>> _children = {};
};
}