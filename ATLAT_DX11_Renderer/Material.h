#pragma once

#include "Object.h"

namespace ATLAT::DX11
{
class Material : public Object
{
public:
	Material();
	virtual ~Material() {};

	void Init(uint32 size);
	void Update();

	void SetShader(std::shared_ptr<Shader> shader) { _shader = shader; }
	void SetTexture(uint8 index, std::shared_ptr<Texture> texture) { _textures[index] = texture; }
	void SetData(const void* data, uint32 size, uint8 offset);

private:
	std::shared_ptr<Shader> _shader;
	std::array<std::shared_ptr<Texture>, 5> _textures;
	std::vector<uint32> _data;

	ComPtr<ID3D11DeviceContext> _deviceContext;
};
}
