#pragma once

#include "Component.h"

namespace ATLAT
{
class Camera : public Component, public std::enable_shared_from_this<Camera>
{
public:
	Camera();
	virtual ~Camera();

	virtual void Awake() override;
	virtual void LateUpdate() override;

	Matrix GetViewMatrix() { return _viewMatrix; }
	Matrix GetProjectionMatrix() { return _projectionMatrix; }

	void SetProjectionMatrix(float fovY, float aspect, float nearZ, float farZ);

	static std::shared_ptr<Camera> GetMainCamera() { return _mainCamera.lock(); }
private:
	float _near = 1.f;
	float _far = 1000.f;
	float _fovY = DirectX::XM_PI * 0.25f;

	Matrix _viewMatrix = {};
	Matrix _projectionMatrix = {};

	float _width = 0.f;
	float _height = 0.f;
	float _aspectRadio = 1.f;

	static std::weak_ptr<Camera> _mainCamera;
};
}

