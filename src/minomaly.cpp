#include "minomaly/minomaly.hpp"
#include "minomaly/input_system.hpp"
#include "minomaly/sdl_subsystems.hpp"
#include "minomaly/window_system.hpp"
#include "minomaly/log_manager.hpp"
#include <algorithm>

using namespace mino;

void Minomaly::update()
{
    for (auto& system : systems)
    {
        system->update();
    }
}

void Minomaly::start()
{
    init();
    running = true;
    for (auto& system : systems)
    {
        system->start();
    }
}

void Minomaly::init()
{
    add_manager(std::make_unique<LogManager>());
    create_system<SdlSubsystems>();
    create_system<WindowSystem>("Minomaly", 0, 0, 800, 600, 0);
    create_system<InputSystem>();
}

void Minomaly::stop()
{
    running = false;
}

void Minomaly::run()
{
    start();
    while (running)
    {
        update();
    }
}

void Minomaly::remove_entity(EntityId id)
{
    auto removed = std::find_if(entities.begin(), entities.end(),
                                [id](auto const& entity) { return entity.id == id; });
    if (removed == entities.end())
    {
        return;
    }
    for (auto& manager : managers)
    {
        manager->remove_component(id);
    }
    entities.erase(removed);
}

Entity& Minomaly::add_entity()
{
    static EntityId NEXT_ID = 0;
    auto& result = entities.emplace_back();
    result.id = NEXT_ID++;
    return result;
}

Minomaly::EntityContainer const& Minomaly::get_entities() const
{
    return entities;
}

void Minomaly::add_manager(std::unique_ptr<IManager>&& manager)
{
    managers.push_back(std::move(manager));
}

