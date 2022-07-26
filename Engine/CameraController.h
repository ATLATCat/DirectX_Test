#pragma once

#include "Component.h"

namespace ATLAT
{
class CameraController : public Component
{
public:
	CameraController();

	virtual void Awake() override;
	virtual void Update() override;

	void Walk(float value);
	void Strafe(float value);
	void UpDown(float value);

	void Pitch(float angle);
	void RotateY(float angle);
private:
	void OnMouseMove(int buttonState, int x, int y);

	float _moveSpeed = 10;
	float _rotationSpeed = 0.01f;
	Input* _input;
	std::shared_ptr<Transform> _transform;
	std::shared_ptr<class Camera> _camera;

	Vector2 _lastMousePosition = Vector2::Zero;
};
}

