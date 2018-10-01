#pragma once

namespace mino
{
class Logger
{
public:
    explicit Logger()
    {
    }

    ~Logger()
    {
    }

    template <typename... Args>
    void trace(const char* fmt, Args... args)
    {
    }

    template <typename... Args>
    void debug(const char* fmt, Args... args)
    {
    }

    template <typename... Args>
    void info(const char* fmt, Args... args)
    {
    }

    template <typename... Args>
    void warning(const char* fmt, Args... args)
    {
    }

    template <typename... Args>
    void error(const char* fmt, Args... args)
    {
    }

    template <typename... Args>
    void fatal(const char* fmt, Args... args)
    {
    }
};
} // namespace mino

