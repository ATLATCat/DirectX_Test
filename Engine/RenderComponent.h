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

#pragma region ���������� ���� ������, �ӽ÷� ������ֱ� ������ ����ȭ ������ �� �� �ٸ� �Ŵ����� �ʿ��� ��
	ConstantData _data;
#pragma endregion
};
}
