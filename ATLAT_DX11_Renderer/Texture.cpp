#include "pch.h"
#include "Texture.h"
#include "Device.h"
#include <filesystem>

namespace fs = std::filesystem;
using namespace DirectX;

void ATLAT::DX11::Texture::LoadTexture(const std::wstring& path)
{
	std::wstring extension = fs::path(path).extension();

	if (extension == L".dds" || extension == L".DDS")
	{
		HR(::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image));
	}
	else if (extension == L".tga" || extension == L".TGA")
	{
		HR(::LoadFromTGAFile(path.c_str(), nullptr, _image));
	}
	else
	{
		HR(::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image));
	}
	HR(::CreateShaderResourceView(Device::GetInstance().GetDevice().Get(),
		_image.GetImages(),
		_image.GetImageCount(), 
		_image.GetMetadata(), 
		&_textureView));

}

