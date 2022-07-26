#include "pch.h"
#include "RenderTarget.h"
#include "Device.h"

void ATLAT::DX11::RenderTarget::Init(ComPtr<ID3D11Device> device, ComPtr<ID3D11Resource> resource)
{
	device->CreateRenderTargetView(resource.Get(), nullptr, _renderTargetView.GetAddressOf());
}

void ATLAT::DX11::RenderTarget::Init(UINT width, UINT height, bool isMSAA)
{
	Device& device = Device::GetInstance();
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	if (isMSAA)
	{
		textureDesc.SampleDesc.Count = 4;
		textureDesc.SampleDesc.Quality = device.GetMASSQuality() - 1;
	}
	else
	{
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
	}

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> textureBuffer;

	HR(device.GetDevice()->CreateTexture2D(&textureDesc, 0, textureBuffer.GetAddressOf()));
	HR(device.GetDevice()->CreateRenderTargetView(textureBuffer.Get(), 0, &_renderTargetView));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	HR(device.GetDevice()->CreateShaderResourceView(textureBuffer.Get(), &shaderResourceViewDesc, _shaderResourceView.GetAddressOf()));
}

ComPtr<ID3D11DepthStencilView> ATLAT::DX11::RenderTarget::CreateDepthStencilView(UINT width, UINT height, bool isMSAA)
{
	Device& device = Device::GetInstance();
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//나중에 어떻게 할지 생각해보자
	if (isMSAA)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = device.GetMASSQuality() - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilBuffer;

	HR(device.GetDevice()->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer));
	ComPtr<ID3D11DepthStencilView> depthStencilView;
	HR(device.GetDevice()->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView));

	return depthStencilView;
}
