#include "pch.h"
#include "CameraController.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Engine.h"

ATLAT::CameraController::CameraController()
	: Component(Type::Action)
{
}

void ATLAT::CameraController::Awake()
{
	_transform = GetTransform();
	_input = &Input::GetInstance();
	_camera = _gameObject.lock()->GetComponent<Camera>();
	_Assert(_camera != nullptr);

	Engine::GetInstance().OnMouseMoveEvent = std::bind(&CameraController::OnMouseMove, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

void ATLAT::CameraController::Update()
{
	float deltaTime = Time::GetInstance().GetDeltaTime();
	float speed = deltaTime * _moveSpeed;

	if (_input->GetKey(vk_alpha('w')))
	{
		Walk(speed);
	}
	else if (_input->GetKey(vk_alpha('s')))
	{
		Walk(-speed);
	}
	if (_input->GetKey(vk_alpha('d')))
	{
		Strafe(speed);
	}
	else if (_input->GetKey(vk_alpha('a')))
	{
		Strafe(-speed);
	}
	if (_input->GetKey(vk_alpha('q')))
	{
		UpDown(-speed);
	}
	else if (_input->GetKey(vk_alpha('e')))
	{
		UpDown(speed);
	}
}

void ATLAT::CameraController::Walk(float value)
{
	_transform->TransformVector(_transform->GetForward() * value);
}

void ATLAT::CameraController::Strafe(float value)
{
	_transform->TransformVector(_transform->GetRight() * value);

}

void ATLAT::CameraController::UpDown(float value)
{
	_transform->TransformVector(_transform->GetUp() * value);

}

void ATLAT::CameraController::Pitch(float angle)
{
	_transform->RotateAxis(_transform->GetRight(), angle);
}

void ATLAT::CameraController::RotateY(float angle)
{
	_transform->RotateAxis({ 0, 1, 0 }, angle);
}

void ATLAT::CameraController::OnMouseMove(int buttonState, int x, int y)
{
	if ((buttonState & MK_RBUTTON) != 0)
	{
		float delta = Time::GetInstance().GetDeltaTime();
		float moveX = (x - _lastMousePosition.x);
		float moveY = (y - _lastMousePosition.y);

		Pitch(moveY * _rotationSpeed);
		RotateY(moveX * _rotationSpeed);
	}
	_lastMousePosition.x = static_cast<float>(x);
	_lastMousePosition.y = static_cast<float>(y);
}
