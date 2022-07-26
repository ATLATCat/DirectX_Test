#pragma once

#include "Object.h"

namespace ATLAT::DX12
{
class Shader;
class Texture;

enum
{
	MATERIAL_INT_COUNT = 5,
	MATERIAL_FLOAT_COUNT = 5,
	MATERIAL_TEXTURE_COUNT = 5,
};

class Material : public Object
{
public:
	Material();
	virtual ~Material() {}

	void Init(int size);

	std::shared_ptr<Shader> GetShader() { return _shader; }

	void SetData(const void* data, uint32 size, uint8 offset);
	void SetShader(std::shared_ptr<Shader> shader) { _shader = shader; }
	void SetTexture(uint8 index, std::shared_ptr<Texture> texture) { _textures[index] = texture; }

	void Update();
private:
	std::wstring _name;

	std::shared_ptr<Shader> _shader;
	std::vector<uint32> _data;
	std::array<std::shared_ptr<Texture>, MATERIAL_TEXTURE_COUNT> _textures;

	std::shared_ptr<ConstantBuffer> _constantBuffer;
	std::shared_ptr<TableDescriptorHeap> _tableDescriptorHeap;
};
}

