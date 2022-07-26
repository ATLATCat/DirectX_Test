#pragma once

#include "Component.h"

namespace ATLAT
{
struct ConstantData
{
	Matrix world;
	Matrix worldView;
	Matrix worldViewProj;
	Vector3 viewPosition;
	float padding;
};

class Mesh;
class Material;

class RenderComponent abstract : public Component
{
public:
	RenderComponent();

	void SetMesh(std::shared_ptr<Mesh> mesh) { _mesh = mesh; }
	void SetMaterial(std::shared_ptr<Material> material) { _material = material; }

protected:
	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;

#pragma region 렌더쪽으로 던질 데이터, 임시로 만들어주긴 했지만 동기화 관리를 해 줄 다른 매니저가 필요할 듯
	ConstantData _data;
#pragma endregion
};
}
