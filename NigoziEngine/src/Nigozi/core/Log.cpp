#include "ngpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Nigozi
{
	std::shared_ptr<spdlog::logger> Log::s_coreLogger;
	std::shared_ptr<spdlog::logger> Log::s_clientLogger;

	void Log::Initialize()
	{
		s_coreLogger = spdlog::stdout_color_mt("Core");
		s_coreLogger->set_level(spdlog::level::trace);
		s_coreLogger->set_pattern("%^[%H:%M:%S] [%n] [%l]%$ %v");

		s_clientLogger = spdlog::stdout_color_mt("Client");
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->set_pattern("%^[%H:%M:%S] [%n] [%l]%$ %v");
	}

	std::shared_ptr<spdlog::logger> Log::CreateLogger(const std::string& name)
	{
		std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(name);
		logger->set_level(spdlog::level::trace);
		logger->set_pattern("%^[%H:%M:%S] [%n] [%l]%$ %v");

		return logger;
	}
}