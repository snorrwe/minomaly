#pragma once
#include "SDL.h"
#include "system.hpp"
#include <vector>

namespace mino
{
using Event = SDL_Event;

class InputSystem final : public ISystem
{
    std::vector<Event> current_events;

public:
    InputSystem();
    ~InputSystem()
    {
    }

    virtual void update() override;
};
} // namespace mino

