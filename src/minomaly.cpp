#include "minomaly/minomaly.hpp"
#include <algorithm>

using namespace mino;

void Minomaly::update()
{
    for (auto& system : systems)
    {
        system->update();
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

std::vector<Entity> const& Minomaly::get_entities() const
{
    return entities;
}

