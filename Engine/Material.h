#pragma once

#include "Object.h"

namespace ATLAT
{
class Shader;
class Texture;

struct MaterialData
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 reflect;
};

class Material : public Object
{
public:
	Material();
	virtual ~Material() { }

	void SetShader(std::shared_ptr<Shader> shader);
	void SetTexture(std::shared_ptr<Texture> texture);

	void SetData(const MaterialData& data);
private:
	void PushData();

	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Material> _material;

	std::shared_ptr<MaterialData> _data;
};
}
