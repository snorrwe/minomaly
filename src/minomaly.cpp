#include "minomaly/minomaly.hpp"
#include "minomaly/system/input_system.hpp"
#include "minomaly/system/render_system.hpp"
#include "minomaly/system/sdl_subsystems.hpp"
#include "minomaly/system/window_system.hpp"
#include <algorithm>
#include <chrono>

using namespace mino;
using milliseconds = std::chrono::duration<double, std::milli>;

std::unique_ptr<LogManager> Minomaly::log_manager = std::make_unique<LogManager>();

void Minomaly::update()
{
    logger->debug("Engine update starting");
    auto start = std::chrono::steady_clock::now();
    for (auto& system : systems)
    {
        system->update();
    }
    milliseconds elapsed = std::chrono::steady_clock::now() - start;
    logger->debug("Engine update finished in {} ms", elapsed.count());
}

void Minomaly::start()
{
    logger->debug("Starting...");
    auto start = std::chrono::steady_clock::now();
    init();
    running = true;
    for (auto& system : systems)
    {
        system->start();
    }
    milliseconds elapsed = std::chrono::steady_clock::now() - start;
    logger->debug("Startup finished in {} ms", elapsed.count());
}

void Minomaly::init()
{
    create_system<SdlSubsystems>();
    create_system<InputSystem>();
    // FIXME let users pass window parameters
    auto window_system = create_system<WindowSystem>("Minomaly", 0, 0, 1920, 1080, 0);
    create_system<RenderSystem>(*window_system->get_window(),
                                log_manager->get_logger("render_system"),
                                *get_or_create_manager<Manager<RenderComponent>>(),
                                *get_or_create_manager<Manager<PositionComponent>>());
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
    auto removed = std::find_if(
        entities.begin(), entities.end(), [id](auto const& entity) { return entity.id == id; });
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
    entities.emplace_back();
    auto& result = entities.back();
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

Minomaly::Minomaly() : logger(log_manager->get_logger("minomaly"))
{
    logger->debug("Created Minomaly");
}

Minomaly::~Minomaly()
{
    logger->debug("Destroying Minomaly");
}

void Minomaly::clear_entities()
{
    for (auto& entity : entities)
    {
        remove_entity(entity.id);
    }
}

