// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <Windows.h>

#pragma comment(lib, "d3d11")

#pragma comment(lib, "RendererCommon.lib")
#include "RendererCommonPch.h"
#pragma comment(lib, "EngineCommon.lib")
#include "EngineCommonPch.h"

#include <d3d11.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_d.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif

#include "DirectXTex.h"
#include "DirectXTex.inl"

#include "DX11Renderer.h"

namespace ATLAT::DX11 {}
using namespace ATLAT::DX11;

#endif //PCH_H
