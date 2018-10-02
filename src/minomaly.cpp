#include "minomaly/minomaly.hpp"
#include "minomaly/input_system.hpp"
#include "minomaly/sdl_subsystems.hpp"
#include "minomaly/window_system.hpp"
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
    logger->debug("Initializing...");
    add_manager(std::move(log_manager));
    create_system<SdlSubsystems>();
    create_system<WindowSystem>("Minomaly", 0, 0, 800, 600, 0);
    create_system<InputSystem>();
    logger->debug("Initialization finished");
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

LogManager* Minomaly::get_log_manager() const
{
    return log_manager.get();
}

Minomaly::Minomaly()
    :logger(log_manager->get_logger("minomaly"))
{
    logger->debug("Created Minomaly");
}

Minomaly::~Minomaly()
{
    logger->debug("Destroying Minomaly");
}

