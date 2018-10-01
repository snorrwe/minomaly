#include "minomaly/log_manager.hpp"

using namespace mino;

LogManager::LogManager()
{
}

Logger* LogManager::get_logger(const char* logger)
{
    auto result = loggers.find(logger);

    if (result == loggers.end())
    {
        // loggers.insert(
        //     std::make_pair(logger, std::make_unique<Logger>(root_logger.getInstance(logger))));
    }
    return loggers[logger].get();
}

