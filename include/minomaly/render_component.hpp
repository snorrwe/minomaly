#pragma once
#include "SDL.h"

namespace mino
{
struct RenderComponent
{
    SDL_Texture* texture = nullptr;
    SDL_Rect source;
    SDL_Rect dest;
};
} // namespace mino

