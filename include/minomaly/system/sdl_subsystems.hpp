#pragma once
#include "system.hpp"

namespace mino
{

enum class SdlStatus
{
    Error = 0,
    Initialized = 1,
    UnInitialized = 2,
};

class SdlSubsystems : public ISystem
{
public:
    SdlSubsystems();
    ~SdlSubsystems();

    SdlSubsystems(SdlSubsystems const&) = delete;
    SdlSubsystems& operator=(SdlSubsystems const&) = delete;

private:
    static void Init_SDL();
    static void Init_SDL_image();
    static void Init_SDL_ttf();
    static void Init_SDL_mixer();
};
} // namespace mino

