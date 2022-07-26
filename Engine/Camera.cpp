#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Engine.h"

std::weak_ptr<ATLAT::Camera> ATLAT::Camera::_mainCamera;

ATLAT::Camera::Camera()
	: Component(Type::Camera)
{
}

ATLAT::Camera::~Camera()
{
	if (_mainCamera.expired())
	{
		_mainCamera.reset();
	}
}

void ATLAT::Camera::Awake()
{
	if (_mainCamera.lock() == nullptr)
	{
		_mainCamera = shared_from_this();
	}

	_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(_fovY, _aspectRadio, _near, _far);
}

void ATLAT::Camera::LateUpdate()
{
	// 직교 보정 구현 필요함.

	_viewMatrix = GetTransform()->GetWorldMatrix().Invert();
}

void ATLAT::Camera::SetProjectionMatrix(float fovY, float aspect, float nearZ, float farZ)
{
	_fovY = fovY;
	_aspectRadio = aspect;
	_near = nearZ;
	_far = farZ;

	_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(_fovY, _aspectRadio, _near, _far);
}

