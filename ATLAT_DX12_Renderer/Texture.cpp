#include "pch.h"
#include "Texture.h"
#include <filesystem>
#include "CommandQueue.h"
#include "Device.h"

namespace fs = std::filesystem;
using namespace DirectX;

void ATLAT::DX12::Texture::Init(const std::wstring& path)
{
	_device = DX12Renderer::GetInstance()->GetDevice()->GetDeivce();
	CreateTexture(path);
	CreateView();
}

void ATLAT::DX12::Texture::CreateTexture(const std::wstring& path)
{
	std::wstring extension = fs::path(path).extension();

	if (extension == L".dds" || extension == L".DDS")
		::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, _image);
	else if (extension == L".tga" || extension == L".TGA")
		::LoadFromTGAFile(path.c_str(), nullptr, _image);
	else
		::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, _image);

	HR(::CreateTexture(_device.Get(), _image.GetMetadata(), &_tex2d));

	std::vector<D3D12_SUBRESOURCE_DATA> subResource;
	HR(::PrepareUpload(_device.Get(), 
		_image.GetImages(),
		_image.GetImageCount(), 
		_image.GetMetadata(), 
		subResource));
	
	const uint64 bufferSize = ::GetRequiredIntermediateSize(_tex2d.Get(), 0,
		static_cast<uint32>(subResource.size()));

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> textureUploadHeap;
	HR(_device->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(textureUploadHeap.GetAddressOf())));

	UpdateSubresources(DX12Renderer::GetInstance()->GetCommandQueue()->GetRsourceCommandList().Get(),
		_tex2d.Get(),
		textureUploadHeap.Get(),
		0, 0,
		static_cast<unsigned int>(subResource.size()),
		subResource.data());

	DX12Renderer::GetInstance()->GetCommandQueue()->FlushResourceCommandQueue();
}

void ATLAT::DX12::Texture::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HR(_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&_srvHeap)));

	_srvHandle = _srvHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = _image.GetMetadata().format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MipLevels = 1;
	_device->CreateShaderResourceView(_tex2d.Get(), &srvDesc, _srvHandle);
}
