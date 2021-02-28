#pragma once
#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Popeye {

	class POPEYE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//core log macro
#define POPEYE_CORE_TRACE(...)		::Popeye::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define POPEYE_CORE_INFO(...)		::Popeye::Log::GetCoreLogger()->info(__VA_ARGS__)
#define POPEYE_CORE_WARN(...)		::Popeye::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define POPEYE_CORE_ERROR(...)		::Popeye::Log::GetCoreLogger()->error(__VA_ARGS__)
#define POPEYE_CORE_FATAL(...)		::Popeye::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//client log macro
#define POPEYE_TRACE(...)			::Popeye::Log::GetClientLogger()->trace(__VA_ARGS__)
#define POPEYE_INFO(...)			::Popeye::Log::GetClientLogger()->info(__VA_ARGS__)
#define POPEYE_WARN(...)			::Popeye::Log::GetClientLogger()->warn(__VA_ARGS__)
#define POPEYE_ERROR(...)			::Popeye::Log::GetClientLogger()->error(__VA_ARGS__)
#define POPEYE_FATAL(...)			::Popeye::Log::GetClientLogger()->fatal(__VA_ARGS__)

