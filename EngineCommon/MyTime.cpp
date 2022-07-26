#include "pch.h"
#include "MyTime.h"

using namespace std::chrono;

void ATLAT::Time::Init()
{
	_startTime = steady_clock::now();
	_preTime = _startTime;
	_frameTime = 0.f;
	_frameCount = 0;
}

void ATLAT::Time::Update()
{
	auto now = steady_clock::now();
	duration<double> duration = now - _preTime;
	_preTime = now;
	_deltaTime = duration.count();

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime > 1.f)
	{
		_fps = static_cast<uint32_t>(_frameCount / _frameTime);
		_frameTime = 0;
		_frameCount = 0;
	}
}

float ATLAT::Time::GetDeltaTime()
{
	return static_cast<float>(_deltaTime);
}

uint32_t ATLAT::Time::GetFps()
{
	return _fps;
}
