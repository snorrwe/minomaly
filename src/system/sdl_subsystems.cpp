#include "minomaly/system/sdl_subsystems.hpp"
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <iostream>

using namespace mino;

SdlSubsystems::SdlSubsystems()
{
    Init_SDL();
    Init_SDL_image();
    Init_SDL_ttf();
    Init_SDL_mixer();
}

SdlSubsystems ::~SdlSubsystems()
{
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SdlSubsystems::Init_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error:\n";
        std::cout << SDL_GetError();
    }
}

void SdlSubsystems::Init_SDL_image()
{
    auto img_flags = IMG_INIT_PNG;
    if ((!IMG_Init(img_flags)) & img_flags)
    {
        std::cout << ("SDL_image could not initialize! SDL_image Error:\n");
        std::cout << (IMG_GetError());
    }
}

void SdlSubsystems::Init_SDL_ttf()
{
    if (TTF_Init() < 0)
    {
        std::cout << ("SDL_ttf could not initialize! SDL_ttf Error: \n");
        std::cout << (TTF_GetError());
    }
}

void SdlSubsystems::Init_SDL_mixer()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << ("SDL_mixer could not initialize! SDL_mixer Error: \n");
        std::cout << (Mix_GetError());
    }
}

