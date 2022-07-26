#pragma once

#include "RenderComponent.h"

namespace ATLAT
{
class Mesh;
class Material;

class MeshRenderer : public RenderComponent
{
public:
	MeshRenderer();

	virtual void Awake() override;
	virtual void LateUpdate() override;

private:

	std::shared_ptr<Transform> _transform;
};
}

