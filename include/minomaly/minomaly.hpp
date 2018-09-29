#pragma once
#include "entity.hpp"
#include "manager.hpp"
#include "system.hpp"
#include <memory>
#include <vector>

namespace mino
{
class Minomaly
{

public:
    using EntityContainer = std::vector<Entity>;
    using SystemContainer = std::vector<std::unique_ptr<ISystem>>;
    using ManagerContainer = std::vector<std::unique_ptr<IManager>>;

    Minomaly() = default;
    Minomaly(Minomaly const&) = delete;
    ~Minomaly() = default;
    Minomaly& operator=(Minomaly const&) = delete;

    /// Run the game loop
    void run();

    template <typename TSys, typename... Args>
    TSys* create_system(Args&&... args);

    // TODO: make managers internal by default?
    // TODO: add option to define custom manager
    template <typename TComponent>
    Manager<TComponent>* create_component_manager();

    Entity& add_entity();
    void remove_entity(EntityId id);

    void start();
    void update();
    void stop();

    EntityContainer const& get_entities() const;

    template <typename TManager>
    TManager* get_manager() const;

    template <typename TSys>
    TSys* get_system() const;

private:
    ManagerContainer managers;
    SystemContainer systems;
    EntityContainer entities;

    bool running = false;

    void init();
};

template <typename TSys, typename... Args>
TSys* Minomaly::create_system(Args&&... args)
{
    auto sys = std::make_unique<TSys>(std::forward<Args>(args)...);
    auto result = sys.get();
    systems.emplace_back(std::move(sys));

    if (running)
    {
        // If the system is already running call the start method right away
        result->start();
    }
    return result;
}

template <typename TComponent>
Manager<TComponent>* Minomaly::create_component_manager()
{
    auto manager = std::make_unique<Manager<TComponent>>();
    auto result = manager.get();
    managers.emplace_back(std::move(manager));
    return result;
}

template <typename TManager>
TManager* Minomaly::get_manager() const
{
    auto result = std::find_if(managers.begin(), managers.end(),
                               [](auto& manager) { return typeid(*manager) == typeid(TManager); });
    if (result != managers.end())
    {
        return static_cast<TManager*>(result->get());
    }
    return nullptr;
}

template <typename TSys>
TSys* Minomaly::get_system() const
{
    auto result = std::find_if(systems.begin(), systems.end(),
                               [](auto& system) { return typeid(*system) == typeid(TSys); });
    if (result != systems.end())
    {
        return static_cast<TSys*>(result->get());
    }
    return nullptr;
}

} // namespace mino

