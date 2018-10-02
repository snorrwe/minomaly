#pragma once
#include "system.hpp"

struct SDL_Window;
struct SDL_Surface;

namespace mino
{

class WindowSystem final : public ISystem
{
    SDL_Window* window = nullptr;
    SDL_Surface* surface = nullptr;

public:
    WindowSystem(const char* title, int x, int y, int w, int h, int flag);
    virtual ~WindowSystem();
    WindowSystem(WindowSystem &&);
    WindowSystem& operator=(WindowSystem &&);

    WindowSystem(WindowSystem const&) = delete;
    WindowSystem& operator=(WindowSystem const&) = delete;

    SDL_Window* get_window() const;
    SDL_Surface* get_surface() const;
};
} // namespace mino

