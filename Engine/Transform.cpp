#include "pch.h"
#include "Transform.h"

ATLAT::Transform::Transform()
	: Component(Type::Action)
{
}

ATLAT::Transform::~Transform()
{
}

Vector3 ATLAT::Transform::GetLocalPosition()
{
	return _localPosition;
}

Vector3 ATLAT::Transform::GetPosition()
{
	return _worldMatrix.Translation();
}

void ATLAT::Transform::TransformVector(Vector3 vector)
{
	_localPosition += vector;
	_localMatrix.Translation(_localPosition);
	_worldMatrix.Translation(_localPosition);
}

void ATLAT::Transform::RotateAxis(Vector3 axis, float angle)
{
	Matrix rotation = Matrix::CreateFromAxisAngle(axis, angle);
	_forward = Vector3::TransformNormal(_forward, rotation);
	_right = Vector3::TransformNormal(_right, rotation);
	_up = Vector3::TransformNormal(_up, rotation);

	_worldMatrix.Forward(_forward);
	_worldMatrix.Right(_right);
	_worldMatrix.Up(_up);
}

Matrix ATLAT::Transform::GetLocalMatrix()
{
	return _localMatrix;
}

Matrix ATLAT::Transform::GetWorldMatrix()
{
	return _worldMatrix;
}

Vector3 ATLAT::Transform::GetRight()
{
	return _worldMatrix.Right();
}

Vector3 ATLAT::Transform::GetForward()
{
	return _worldMatrix.Backward();
}

Vector3 ATLAT::Transform::GetUp()
{
	return _worldMatrix.Up();
}
