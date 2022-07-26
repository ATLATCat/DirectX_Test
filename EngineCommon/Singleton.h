#pragma once

template <typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T* _instance = new T();

		return *_instance;
	}
};

