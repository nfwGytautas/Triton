#pragma once

#include "Triton/TRMacros.h"
#include "spdlog/spdlog.h"

namespace Triton {

	struct LogMessage
	{
		std::string Format;
		std::string Text;
		spdlog::level::level_enum Level;
	};

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& getGUILogger() { return s_GUILogger; }

		inline static std::shared_ptr<spdlog::logger>& getSystemLogger() { return s_SystemLogger; }

		// History of the logger used for GUI log
		static std::vector<LogMessage> History;
	private:
		// GUI logger logs messages to GUI console
		static std::shared_ptr<spdlog::logger> s_GUILogger;

		// System logger logs messages to console rather than the GUI log
		static std::shared_ptr<spdlog::logger> s_SystemLogger;
	};

}

// System log macros
#define TR_SYSTEM_TRACE(...)       ::Triton::Log::getSystemLogger()->trace(__VA_ARGS__)
#define TR_SYSTEM_INFO(...)        ::Triton::Log::getSystemLogger()->info(__VA_ARGS__)
#define TR_SYSTEM_WARN(...)        ::Triton::Log::getSystemLogger()->warn(__VA_ARGS__)
#define TR_SYSTEM_ERROR(...)       ::Triton::Log::getSystemLogger()->error(__VA_ARGS__)

// GUI log macros
#define TR_TRACE(...)       ::Triton::Log::getGUILogger()->trace(__VA_ARGS__)
#define TR_INFO(...)        ::Triton::Log::getGUILogger()->info(__VA_ARGS__)
#define TR_WARN(...)        ::Triton::Log::getGUILogger()->warn(__VA_ARGS__)
#define TR_ERROR(...)       ::Triton::Log::getGUILogger()->error(__VA_ARGS__)