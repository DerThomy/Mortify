#pragma once

#include "Mortify/Debug/DebugBreak.h"

#include <memory>

#ifdef _WIN32
	/* Windows x64/x86 */
#ifdef _WIN64
	/* Windows x64  */
	#define MT_PLATFORM_WINDOWS
#else
	/* Windows x86 */
	#error "x86 Builds aren't supported"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	/* TARGET_OS_MAC exists on all the platforms
	 * so we must check all of them (in this order)
	 * to ensure that we're running on MAC
	 * and not some other Apple platform */
	#if TARGET_IPHONE_SIMULATOR == 1
	#error "IOS simulator is not supported"
	#elif TARGET_OS_IPHONE == 1
	#error "IOS is not supported"
	#elif TARGET_OS_MAC == 1
	#define HZ_PLATFORM_MACOS
	#else
	#error "Unknown Apple platform"
	#endif
	/* We also have to check __ANDROID__ before __linux__
	 * since android is based on the linux kernel
	 * it has __linux__ defined */
#elif defined(__ANDROID__)
	#error "Android is not supported!"
#elif defined(__linux__)
	#define MT_PLATFORM_LINUX
    #define GLFW_EXPOSE_NATIVE_GLX
#else
	/* Unknown compiler/platform */
	#error "Unknown platform!"
#endif

#ifdef MT_DEBUG
	#define MT_ENABLE_ASSERTS
#endif

#ifdef MT_ENABLE_ASSERTS
	#define MT_ASSERT(x, ...)		{ if(!(x)) { MT_ERROR("Assertion Failed: {0}", __VA_ARGS__); debugbreak(); } }
	#define MT_CORE_ASSERT(x, ...)	{ if(!(x)) { MT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); debugbreak(); } }
#else
	#define MT_ASSERT(x, ...)
	#define MT_CORE_ASSERT(x, ...)
#endif

#define MT_DONT_CARE -1

#define BIT(x) (1 << x)

#define MT_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Mortify
{
	using byte = unsigned char;

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
}