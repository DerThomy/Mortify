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

#ifdef MT_DEBUG
	#define MT_ENABLE_ASSERT
#endif

#ifdef MT_ENABLE_ASSERT
	#define MT_ASSERT(x, ...)		{ if(!(x)) { MT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MT_CORE_ASSERT(x, ...)	{ if(!(x)) { MT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MT_ASSERT(x, ...)
	#define MT_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)