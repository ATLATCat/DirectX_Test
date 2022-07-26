#include "pch.h"
#include "Shader.h"
#include "DepthStencilBuffer.h"
#include "CommandQueue.h"
#include "Device.h"
#include "RootSignature.h"

void ATLAT::DX12::Shader::Init(const std::wstring& path)
{
	_commandList = DX12Renderer::GetInstance()->GetCommandQueue()->GetCommandList();
	CreateVertexShader(path, "VS_Main", "vs_5_0");
	CreatePixelShader(path, "PS_Main", "ps_5_0");

	D3D12_INPUT_ELEMENT_DESC desc[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	_pipelineDesc.InputLayout = { desc, _countof(desc) };
	_pipelineDesc.pRootSignature = DX12Renderer::GetInstance()->GetRootSignature()->GetSignature().Get();

	_pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	_pipelineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	_pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	_pipelineDesc.SampleMask = UINT_MAX;
	_pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	_pipelineDesc.NumRenderTargets = 1;
	_pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	_pipelineDesc.SampleDesc.Count = 1;
	_pipelineDesc.DSVFormat = DX12Renderer::GetInstance()->GetDepthStencilBuffer()->GetFormat();

	ComPtr<ID3D12Device> device = DX12Renderer::GetInstance()->GetDevice()->GetDeivce();
	HR(device->CreateGraphicsPipelineState(&_pipelineDesc, IID_PPV_ARGS(&_pipelineState)));
}

void ATLAT::DX12::Shader::Update()
{
	_commandList->SetPipelineState(_pipelineState.Get());
}

void ATLAT::DX12::Shader::CreateShader(const std::wstring& path, const std::string& name, const std::string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode)
{
	uint32 compileFlag = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	compileFlag |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG
	HR(::D3DCompileFromFile(path.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), version.c_str(), compileFlag, 0, &blob, &_errorBlob));

	shaderByteCode = { blob->GetBufferPointer(), blob->GetBufferSize() };
}

void ATLAT::DX12::Shader::CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version)
{ 
	CreateShader(path, name, version, _vsBlob, _pipelineDesc.VS);
}

void ATLAT::DX12::Shader::CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version)
{
	CreateShader(path, name, version, _psBlob, _pipelineDesc.PS);
}
