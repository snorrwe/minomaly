#pragma once
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace mino
{
using Logger = spdlog::logger;
using LogLevel = spdlog::level::level_enum;

class LogManager
{
    static constexpr const char* DEFAULT_LOGGER = "minomaly";

    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;

public:
    static void set_level(LogLevel level);

    LogManager();
    virtual ~LogManager()
    {
    }

    LogManager(LogManager const&) = delete;
    LogManager& operator=(LogManager const&) = delete;

    Logger* get_logger(const char* logger = DEFAULT_LOGGER);
};
} // namespace mino

