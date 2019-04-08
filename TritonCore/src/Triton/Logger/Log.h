#pragma once

#include "Triton/TRMacros.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Triton {

	class  Log
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

//Core log macros
#define TR_CORE_TRACE(...)  ::Triton::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TR_CORE_INFO(...)   ::Triton::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TR_CORE_WARN(...)   ::Triton::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TR_CORE_ERROR(...)  ::Triton::Log::GetCoreLogger()->error(__VA_ARGS__)

//Client log macros
#define TR_TRACE(...)       ::Triton::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TR_INFO(...)        ::Triton::Log::GetClientLogger()->info(__VA_ARGS__)
#define TR_WARN(...)        ::Triton::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TR_ERROR(...)       ::Triton::Log::GetClientLogger()->error(__VA_ARGS__)