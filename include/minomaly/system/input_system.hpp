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
    InputSystem(InputSystem const&) = default;
    InputSystem(InputSystem&&) = default;
    ~InputSystem() override;

    InputSystem& operator=(InputSystem const&) = default;
    InputSystem& operator=(InputSystem&&) = default;

    void update() override;

    template <typename TFun>
    void iter_events(TFun&& callback);

    std::vector<Event> const& get_events() const;
};

template <typename TFun>
void InputSystem::iter_events(TFun&& callback)
{
    for (auto& event : current_events)
    {
        callback(event);
    }
}

} // namespace mino

