#pragma once
#include "spdlog/spdlog.h"

namespace Triton {

	struct LogMessage
	{
		std::string Format;
		std::string Text;
		spdlog::level::level_enum Level;
	};

	/**
	 * The logger class used by Triton
	 */
	class Log
	{
	public:
		/**
		 * Initializes Triton logger
		 */
		static void init();

		/**
		 * Flush all logs to files
		 */
		static void flush();

		/**
		 * Set the level of debug messages to be printed to the console
		 *
		 * @param level The spdlog level of the message
		 */
		static void setLevel(spdlog::level::level_enum level);

		inline static std::shared_ptr<spdlog::logger>& getGUILogger() { return s_GUILogger; }

		inline static std::shared_ptr<spdlog::logger>& getSystemLogger() { return s_SystemLogger; }

		/// History of the logger used for GUI log
		static std::vector<LogMessage> History;
	private:
		/// GUI logger logs messages to GUI console
		static std::shared_ptr<spdlog::logger> s_GUILogger;

		/// System logger logs messages to console rather than the GUI log
		static std::shared_ptr<spdlog::logger> s_SystemLogger;
	};
}

// System log macros
#define TR_SYSTEM_DEBUG(...)       ::Triton::Log::getSystemLogger()->debug(__VA_ARGS__)
#define TR_SYSTEM_TRACE(...)       ::Triton::Log::getSystemLogger()->trace(__VA_ARGS__)
#define TR_SYSTEM_INFO(...)        ::Triton::Log::getSystemLogger()->info(__VA_ARGS__)
#define TR_SYSTEM_WARN(...)        ::Triton::Log::getSystemLogger()->warn(__VA_ARGS__)
#define TR_SYSTEM_ERROR(...)       ::Triton::Log::getSystemLogger()->error(__VA_ARGS__)

// GUI log macros
#define TR_TRACE(...)       ::Triton::Log::getGUILogger()->trace(__VA_ARGS__)
#define TR_INFO(...)        ::Triton::Log::getGUILogger()->info(__VA_ARGS__)
#define TR_WARN(...)        ::Triton::Log::getGUILogger()->warn(__VA_ARGS__)
#define TR_ERROR(...)       ::Triton::Log::getGUILogger()->error(__VA_ARGS__)