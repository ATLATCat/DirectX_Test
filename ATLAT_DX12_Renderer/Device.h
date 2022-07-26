#pragma once


namespace ATLAT::DX12
{
class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGIFactory() { return _dxgi; }
	ComPtr<ID3D12Device> GetDeivce() { return _device; }
private:
	ComPtr<ID3D12Debug> _debugController;
	ComPtr<IDXGIFactory> _dxgi;
	ComPtr<ID3D12Device> _device;
};
}