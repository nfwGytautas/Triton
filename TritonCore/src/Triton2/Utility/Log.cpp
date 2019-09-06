#include "TRpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include <mutex>


template<typename Mutex>
class GUI_sink : public spdlog::sinks::base_sink<Mutex>
{
protected:
	void sink_it_(const spdlog::details::log_msg& msg) override
	{
		::Triton::LogMessage lgmsg;

		lgmsg.Text = fmt::to_string(msg.payload);
		lgmsg.Level = msg.level;

		fmt::memory_buffer formatted;
		sink::formatter_->format(msg, formatted);

		std::string formatedStr = fmt::to_string(formatted);

		std::size_t length = formatedStr.find(lgmsg.Text);

		lgmsg.Format = formatedStr.substr(0, length);

		::Triton::Log::History.push_back(lgmsg);
	}

	void flush_() override
	{
		std::cout << std::flush;
	}
};

using GUI_sink_mt = GUI_sink<std::mutex>;
using GUI_sink_st = GUI_sink<spdlog::details::null_mutex>;


namespace Triton {

	std::vector<LogMessage> Log::History;

	std::shared_ptr<spdlog::logger> Log::s_GUILogger;
	std::shared_ptr<spdlog::logger> Log::s_SystemLogger;

	void Log::init()
	{
		std::shared_ptr<GUI_sink_mt> s_GUISink = std::make_shared<GUI_sink_mt>();

		s_GUISink->set_pattern("[%T] Thread: %t %n: %^[%l] %v%$");
		spdlog::set_pattern("[%T] Thread: %t %n: %^[%l] %v%$");

		s_GUILogger = std::make_shared<spdlog::logger>("TRITON", s_GUISink);

		s_SystemLogger = spdlog::stdout_color_mt("SYSTEM");
		s_SystemLogger->set_level(spdlog::level::trace);

		//s_GUILogger = spdlog::stdout_color_mt("TRITON");
		s_GUILogger->set_level(spdlog::level::trace);
	}

}