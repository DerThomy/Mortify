#pragma once

#include "Mortify/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Mortify
{
	class Log
	{
	public:
		static void Init();

		static inline Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		static inline Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger;  }

	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define MT_CORE_TRACE(...)		::Mortify::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MT_CORE_INFO(...)		::Mortify::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MT_CORE_WARN(...)		::Mortify::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MT_CORE_ERROR(...)		::Mortify::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MT_CORE_CRITICAL(...)		::Mortify::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MT_TRACE(...)			::Mortify::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MT_INFO(...)			::Mortify::Log::GetClientLogger()->info(__VA_ARGS__)
#define MT_WARN(...)			::Mortify::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MT_ERROR(...)			::Mortify::Log::GetClientLogger()->error(__VA_ARGS__)
#define MT_CRITICAL(...)			::Mortify::Log::GetClientLogger()->critical(__VA_ARGS__)