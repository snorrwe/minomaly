#include "minomaly/system/window_system.hpp"
#include "SDL.h"
#include <iostream>
#include <stdexcept>

using namespace mino;

WindowSystem::WindowSystem(const char* title, int x, int y, int w, int h, int flag)

    : window(SDL_CreateWindow(title, x, y, w, h, flag))
{
    if (window == nullptr)
    {
        auto error = SDL_GetError();
        std::cout << "Window could not be created! SDL_Error:\n" << error << std::endl;
        throw std::runtime_error(error);
    }
    surface = SDL_GetWindowSurface(window);
}

WindowSystem::WindowSystem(WindowSystem&& other)
    : window(other.window)
    , surface(other.surface)
{
    // Make sure the `other` dtor does not close the window
    other.window = nullptr;
}

WindowSystem& WindowSystem::operator=(WindowSystem&& other)
{
    window = other.window;
    surface = other.surface;
    // Make sure the `other` dtor does not close the window
    other.window = nullptr;
    return *this;
}

WindowSystem::~WindowSystem()
{
    if (window)
    {
        SDL_DestroyWindow(window);
    }
}

SDL_Window* WindowSystem::get_window() const
{
    return window;
}

SDL_Surface* WindowSystem::get_surface() const
{
    return surface;
}

