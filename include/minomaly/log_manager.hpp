#pragma once
#include "manager.hpp"
#include "spdlog/spdlog.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace mino
{
using Logger = spdlog::logger;
using LogLevel = spdlog::level::level_enum;

class LogManager : public IManager
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

    virtual void reserve(size_t size) override
    {
        loggers.reserve(size);
    };

    virtual bool remove_component(EntityId) override
    {
        return false;
    }

    Logger* get_logger(const char* logger = DEFAULT_LOGGER);
};
} // namespace mino

