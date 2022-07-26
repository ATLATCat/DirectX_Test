#pragma once

#include "Object.h"

namespace ATLAT::DX11 {
class Texture : public Object
{
public:
	void LoadTexture(const std::wstring& path);

	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return _textureView; }
private:
	ComPtr<ID3D11ShaderResourceView> _textureView;
	DirectX::ScratchImage _image;
};
}