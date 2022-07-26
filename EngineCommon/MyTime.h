#pragma once

#include "Singleton.h"
#include <chrono>

namespace ATLAT
{
class Time : public Singleton<Time>
{
public:
	void Init();
	void Update();

	float GetDeltaTime();
	uint32_t GetFps();

private:
	uint32_t _frameCount;
	std::chrono::steady_clock::time_point _startTime;
	std::chrono::steady_clock::time_point _preTime;
	double _deltaTime;
	double _frameTime;
	uint32_t _fps;

private:
	Time() = default;
	friend Singleton;
};
}
