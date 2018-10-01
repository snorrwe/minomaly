#pragma once
#include "logger.hpp"
#include "manager.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace mino
{
class LogManager : public IManager
{
    static constexpr const char* DEFAULT_LOGGER = "minomaly";

    std::unordered_map<std::string, std::unique_ptr<Logger>> loggers;

public:
    LogManager();
    ~LogManager()
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

