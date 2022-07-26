#pragma once


#include <Windows.h>

#pragma comment(lib, "d3d12")

#pragma comment(lib, "RendererCommon.lib")
#include "RendererCommonPch.h"
#pragma comment(lib, "EngineCommon.lib")
#include "EngineCommonPch.h"

#include "d3dx12.h"
#include <d3d12.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_d.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif

#include "DirectXTex.h"
#include "DirectXTex.inl"

enum class CBV_REGISTER : uint8
{
	b0,
	b1,
	b2,
	b3,
	b4,
	End
};

enum class SRV_REGISTER : uint8
{
	t0 = static_cast<uint8>(CBV_REGISTER::End),
	t1,
	t2,
	t3,
	t4,
	End
};

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
	CBV_REGISTER_COUNT = CBV_REGISTER::End,
	SRV_REGISTER_COUNT = static_cast<uint8>(SRV_REGISTER::End) - static_cast<uint8>(CBV_REGISTER_COUNT),
	REGISTER_COUNT = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT,
};