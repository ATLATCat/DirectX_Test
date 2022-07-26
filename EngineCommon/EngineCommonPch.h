#pragma once


#include <Windows.h>
#include <memory>
#include <vector>
#include <array>
#include <functional>


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;

using UID = uint64;

struct WindowInfo
{
	HWND hwnd;
	int32 width;
	int32 height;
	bool windowed;
};

#include "Macro.h"
#include "MyTime.h"
#include "Input.h"

#include "InterfaceData.h"