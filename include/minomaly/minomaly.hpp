#pragma once
#include "minomaly/component.hpp"
#include "minomaly/entity.hpp"
#include "minomaly/manager.hpp"
#include "minomaly/system.hpp"
#include <memory>
#include <vector>

namespace mino
{
class Minomaly
{

public:
    using EntityContainer = std::vector<Entity>;
    Minomaly() = default;
    Minomaly(Minomaly const&) = delete;
    ~Minomaly() = default;
    Minomaly& operator=(Minomaly const&) = delete;

    template <typename TSys, typename... Args>
    TSys* create_system(Args&&... args);

    // TODO: make managers internal by default?
    // TODO: add option to define custom manager
    template <typename TComponent>
    Manager<TComponent>* create_component_manager();

    Entity& add_entity();
    void remove_entity(EntityId id);
    void update();

    std::vector<Entity> const& get_entities() const
    {
        return entities;
    }

private:
    std::vector<std::unique_ptr<IManager>> managers;
    std::vector<std::unique_ptr<ISystem>> systems;
    EntityContainer entities;
};

template <typename TSys, typename... Args>
TSys* Minomaly::create_system(Args&&... args)
{
    auto sys = std::make_unique<TSys>(std::forward<Args>(args)...);
    auto result = sys.get();
    systems.emplace_back(std::move(sys));
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

} // namespace mino

