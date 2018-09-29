#include "minomaly/input_system.hpp"

using namespace mino;

InputSystem::InputSystem()
{
    current_events.reserve(10);
}

void InputSystem::update()
{
    current_events.clear();
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        current_events.push_back(e);

        switch (e.type)
        {
            // TODO
        case SDL_QUIT:
            break;
        case SDL_KEYDOWN:
            break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:
            break;
        }
    }
}

