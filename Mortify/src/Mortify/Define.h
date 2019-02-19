#pragma once

#ifdef MT_PLATFORM_WINDOWS
	#ifdef MT_BUILD_DLL
		#define MT_API __declspec(dllexport)
	#else
		#define MT_API __declspec(dllimport)
	#endif
#else
	#error Only Windows Support
#endif

#define BIT(x) (1 << x)