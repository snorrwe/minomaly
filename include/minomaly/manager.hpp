#pragma once
#include "entity.hpp"
#include <algorithm>
#include <cassert>
#include <memory>
#include <tuple>
#include <vector>

namespace mino
{
/// Manages Component memory and lifetimes
class IManager
{
public:
    virtual ~IManager() = default;
    virtual void reserve(size_t size) = 0;
    virtual size_t size() const = 0;
    virtual bool remove_component(EntityId id) = 0;
};

/// Implementation of a concrete `IManager`
template <typename TComponent,
          typename TComponentContainer = std::vector<std::tuple<EntityId, TComponent>>>
class Manager : public IManager
{
    TComponentContainer components;
    EntityId front = 0;
    EntityId back = 0;

public:
    /// Reserve memory for `size` >= 0 components
    void reserve(size_t size) override;

    size_t size() const override;

    /// Add a component to the entity by `id` and return a reference to it
    /// This method keeps the inner storage of components sorted by their entity ids
    /// When using it on a range of `entities` sort the range first for optimal performance
    TComponent& add_component(EntityId id);

    /// Get a pointer to the component of the given id
    /// Return `nullptr` if no component by the given id exists
    /// TODO: fix / ease performance
    TComponent* get_component(EntityId id);

    /// Get an existing component or add a new one if none was found
    /// Note that this method has a slight overhead, so if you know for sure wether or not the
    /// component exists Use the appropriate method for increased performance
    TComponent& get_or_add_component(EntityId id);

    /// Erase the component belonging to the entity
    /// Returns if a component was found and removed by this id
    /// Note that when the Component destructor is called `this` will point to different memory
    /// from it's original. For this reason and more Components should not manage resources
    bool remove_component(EntityId id) override;

    /// Apply a function to all components in the `Manager`
    /// required signature: void foo(EntityId id, TComponent& component);
    /// (the return value is ignored)
    template <typename TFun>
    void iter(TFun&& callback);

    /// Get a reference to the component container
    TComponentContainer& get_components();

    /// Get a reference to the component container
    TComponentContainer const& get_components() const;
};
template <typename T, typename C>
void Manager<T, C>::reserve(size_t size)
{
    components.reserve(size);
}

template <typename T, typename C>
T& Manager<T, C>::add_component(EntityId const id)
{
    auto position =
        std::find_if(components.rbegin(), components.rend(), [id](auto const& component) {
            assert(std::get<0>(component) != id);
            return std::get<0>(component) < id;
        });
    auto len = components.rend() - position;
    auto result = components.emplace(components.begin() + len);
    std::get<0>(*result) = id;
    front = std::get<0>(components.front());
    back = std::get<0>(components.back());
    return std::get<1>(*result);
}

template <typename T, typename C>
T* Manager<T, C>::get_component(EntityId id)
{
    if (id < front || id > back)
    {
        // out of bounds
        return nullptr;
    }
    auto guess = components.rbegin();
    auto end = components.rend();
    auto result = std::find_if(
        guess, end, [id](auto const& component) { return std::get<0>(component) == id; });
    if (result != end)
    {
        return &std::get<1>(*result);
    }
    return nullptr;
}

template <typename T, typename C>
T& Manager<T, C>::get_or_add_component(EntityId id)
{
    auto existing = get_component(id);
    if (existing != nullptr)
    {
        return *existing;
    }
    return add_component(id);
}

template <typename T, typename C>
bool Manager<T, C>::remove_component(EntityId id)
{
    auto removed = std::find_if(components.begin(), components.end(), [id](auto const& component) {
        return std::get<0>(component) == id;
    });
    auto result = removed != components.end();
    if (result)
    {
        components.erase(removed);
    }
    front = std::get<0>(components.front());
    back = std::get<0>(components.back());
    return result;
}

template <typename T, typename C>
template <typename TFun>
void Manager<T, C>::iter(TFun&& callback)
{
    for (auto& component : components)
    {
        callback(std::get<0>(component), std::get<1>(component));
    }
}

template <typename T, typename C>
C& Manager<T, C>::get_components()
{
    return components;
}

template <typename T, typename C>
C const& Manager<T, C>::get_components() const
{
    return components;
}

template <typename T, typename C>
size_t Manager<T, C>::size() const
{
    return components.size();
}

} // namespace mino

