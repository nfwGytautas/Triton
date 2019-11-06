#include "TRpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/details/null_mutex.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <mutex>

#include "Triton2/File/File.h"


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
		const std::string pattern = "[%T.%e] Thread: %-5t %=6n: %^[%=7l] %v%$";

		Triton::IO::createDirectory("logs");

		std::shared_ptr<GUI_sink_mt> s_GUISink = std::make_shared<GUI_sink_mt>();

		auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		console_sink->set_level(spdlog::level::trace);
		console_sink->set_pattern(pattern);

		auto file_sink_system = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/system.txt", true);
		file_sink_system->set_level(spdlog::level::trace);
		file_sink_system->set_pattern(pattern);

		auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/app.txt", true);
		file_sink->set_level(spdlog::level::trace);
		file_sink->set_pattern(pattern);

		s_GUISink->set_pattern(pattern);
		spdlog::set_pattern(pattern);

		s_GUILogger = std::make_shared<spdlog::logger>("TRITON", std::initializer_list<spdlog::sink_ptr>{s_GUISink, file_sink});

		s_SystemLogger = std::make_shared<spdlog::logger>("SYSTEM", std::initializer_list<spdlog::sink_ptr>{console_sink, file_sink_system});;
		s_SystemLogger->set_level(spdlog::level::trace);

		//s_GUILogger = spdlog::stdout_color_mt("TRITON");
		s_GUILogger->set_level(spdlog::level::trace);
	}

	void Log::flush()
	{
		TR_SYSTEM_INFO("FLUSHING");
		TR_INFO("FLUSHING");

		s_SystemLogger->flush();
		s_GUILogger->flush();
	}

	void Log::setLevel(spdlog::level::level_enum level)
	{
		s_SystemLogger->set_level(level);
		s_GUILogger->set_level(level);
	}

}