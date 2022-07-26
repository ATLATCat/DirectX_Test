#pragma once

#include <cassert>

#if defined(DEBUG) | defined(_DEBUG)
#define _Assert(value) (assert(value))
#else
#define _Assert(value) ((void)0)
#endif


#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |			\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |				\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,					\
				NULL,											\
				hr,												\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),		\
				(LPTSTR) &output,								\
				0,												\
				NULL);											\
			MessageBox(NULL, output, L"Error", MB_OK);			\
			_Assert(false);										\
		}														\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 