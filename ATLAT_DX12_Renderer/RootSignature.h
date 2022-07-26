#pragma once

namespace ATLAT::DX12 {
class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature> GetSignature() { return _signature; }
private:
	void CreateSmaplerDesc();
	void CreateRootSignature(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature> _signature;
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};
}