#include "minomaly/log_manager.hpp"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

using namespace mino;

LogManager::LogManager()
{
}

Logger* LogManager::get_logger(const char* logger)
{
    auto result = loggers.find(logger);

    if (result == loggers.end())
    {
        loggers.insert(std::make_pair(logger, spdlog::stdout_color_mt(logger)));
    }
    return loggers[logger].get();
}

void LogManager::set_level(LogLevel level)
{
    spdlog::set_level(level);
}

