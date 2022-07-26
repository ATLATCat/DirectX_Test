#pragma once

namespace ATLAT
{
enum class CONSTANT_BUFFER_TYPE : uint8
{
	Global,
	Transform,
	Material,
	END
};

struct RendererInitData
{
	std::wstring resourcesPath;
	WindowInfo windowInfo;
};
}
