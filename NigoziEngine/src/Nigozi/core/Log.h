#pragma once

#include <memory>
#include "spdlog/spdlog.h"

namespace Nigozi
{
	class Application;

	class Log
	{
	public:
		static void Initialize();

		static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& name);

		inline static std::shared_ptr<spdlog::logger> GetLogger(const std::string& name) { return spdlog::get(name); }
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

#ifndef DISTRIBUTION
#define NG_CORE_LOG_TRACE(...) Nigozi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NG_CORE_LOG_INFO(...) Nigozi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NG_CORE_LOG_WARN(...) Nigozi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NG_CORE_LOG_ERROR(...) Nigozi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define NG_CORE_LOG_CRITICAL(...) Nigozi::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define NG_CLIENT_LOG_TRACE(...) Nigozi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NG_CLIENT_LOG_INFO(...) Nigozi::Log::GetClientLogger()->info(__VA_ARGS__)
#define NG_CLIENT_LOG_WARN(...) Nigozi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NG_CLIENT_LOG_ERROR(...) Nigozi::Log::GetClientLogger()->error(__VA_ARGS__)
#define NG_CLIENT_LOG_CRITICAL(...) Nigozi::Log::GetClientLogger()->critical(__VA_ARGS__)

#define NG_CUSTOM_LOG_TRACE(name, ...) Nigozi::Log::GetLogger(name)->trace(__VA_ARGS__)
#define NG_CUSTOM_LOG_INFO(name, ...) Nigozi::Log::GetLogger(name)->info(__VA_ARGS__)
#define NG_CUSTOM_LOG_WARN(name, ...) Nigozi::Log::GetLogger(name)->warn(__VA_ARGS__)
#define NG_CUSTOM_LOG_ERROR(name, ...) Nigozi::Log::GetLogger(name)->error(__VA_ARGS__)
#define NG_CUSTOM_LOG_CRITICAL(name, ...) Nigozi::Log::GetLogger(name)->critical(__VA_ARGS__)
#else
#define NG_CORE_LOG_TRACE(...)
#define NG_CORE_LOG_INFO(...)
#define NG_CORE_LOG_WARN(...)
#define NG_CORE_LOG_ERROR(...)
#define NG_CORE_LOG_CRITICAL(...) Nigozi::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define NG_CLIENT_LOG_TRACE(...)
#define NG_CLIENT_LOG_INFO(...)
#define NG_CLIENT_LOG_WARN(...)
#define NG_CLIENT_LOG_ERROR(...)
#define NG_CLIENT_LOG_CRITICAL(...) Nigozi::Log::GetClientLogger()->critical(__VA_ARGS__)

#define NG_CUSTOM_LOG_TRACE(name, ...)
#define NG_CUSTOM_LOG_INFO(name, ...)
#define NG_CUSTOM_LOG_WARN(name, ...)
#define NG_CUSTOM_LOG_ERROR(name, ...)
#define NG_CUSTOM_LOG_CRITICAL(name, ...) Nigozi::Log::GetLogger(name)->critical(__VA_ARGS__)
#endif