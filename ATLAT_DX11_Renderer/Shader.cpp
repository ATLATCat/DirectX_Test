#include "pch.h"
#include "Shader.h"
#include "Device.h"

void ATLAT::DX11::Shader::Init(const std::wstring& path)
{
    CreateVertexShader(path, "VS_Main", "vs_5_0");
    CreatePixelShader(path, "PS_Main", "ps_5_0");
}

void ATLAT::DX11::Shader::InitDeferredShader(const std::wstring& path)
{
	HR(CompileShader(path, "VS_Deferred", "vs_5_0", _vertexBlob.GetAddressOf()));
	HR(Device::GetInstance().GetDevice()->CreateVertexShader(_vertexBlob->GetBufferPointer(),
		_vertexBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()));

	const D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
		   { "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   { "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	HR(Device::GetInstance().GetDevice()->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
		_vertexBlob->GetBufferPointer(), _vertexBlob->GetBufferSize(), _vertexInputLayout.GetAddressOf()));

	CreatePixelShader(path, "PS_Deferred", "ps_5_0");
}

void ATLAT::DX11::Shader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
{
    HR(CompileShader(path, name, version, _vertexBlob.GetAddressOf()));
    HR(Device::GetInstance().GetDevice()->CreateVertexShader(_vertexBlob->GetBufferPointer(),
        _vertexBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()));

    const D3D11_INPUT_ELEMENT_DESC inputLayout[] = {
		   { "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
		   { "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    HR(Device::GetInstance().GetDevice()->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout),
        _vertexBlob->GetBufferPointer(), _vertexBlob->GetBufferSize(), _vertexInputLayout.GetAddressOf()));
}

void ATLAT::DX11::Shader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
{
    HR(CompileShader(path, name, version, _pixelBlob.GetAddressOf()));
    HR(Device::GetInstance().GetDevice()->CreatePixelShader(_pixelBlob->GetBufferPointer(),
        _pixelBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf()));
}

HRESULT ATLAT::DX11::Shader::CompileShader(const std::wstring& path, const std::string& name, const std::string& version, ID3DBlob** blob)
{
    *blob = nullptr;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
    flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE", "1",
        NULL, NULL
    };

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(path.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        name.c_str(), version.c_str(),
        flags, 0, &shaderBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        if (shaderBlob)
            shaderBlob->Release();

        return hr;
    }

    *blob = shaderBlob;

    return hr;
}