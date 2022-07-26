#pragma once

#include "Object.h"

namespace ATLAT::DX11
{
class Shader : public Object
{
public:
	void Init(const std::wstring& path);
	void InitDeferredShader(const std::wstring& path);

	ComPtr<ID3D11VertexShader> GetVertexShader() { return _vertexShader; }
	ComPtr<ID3D11PixelShader> GetPixelShader() { return _pixelShader; }
	ComPtr<ID3D11InputLayout> GetInputLayout() { return _vertexInputLayout; }


private:
	void CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);
	HRESULT CompileShader(const std::wstring& path, const std::string& name, const std::string& version, ID3DBlob** blob);
	
	ComPtr<ID3DBlob> _vertexBlob;
	ComPtr<ID3DBlob> _pixelBlob;

	ComPtr<ID3D11VertexShader> _vertexShader;
	ComPtr<ID3D11PixelShader> _pixelShader;
	ComPtr<ID3D11InputLayout> _vertexInputLayout;
};
}

