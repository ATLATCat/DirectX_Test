#pragma once

#include "RenderComponent.h"

namespace ATLAT
{

class Mesh;
class Material;

class Sprite : public RenderComponent
{
public:
	Sprite();

	virtual void Awake() override;
	virtual void LateUpdate() override;

private:
	void CreateMesh();

	std::shared_ptr<Transform> _transform;
};
}

