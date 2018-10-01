#pragma once
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
    /// Reserve space for n >= 0 components
    virtual void reserve(size_t size) = 0;
    virtual bool remove_component(EntityId id) = 0;
};

template <typename TComponent>
class Manager : public IManager
{
    using TInnerComponent = std::tuple<EntityId, TComponent>;
    using TComponentContainer = std::vector<TInnerComponent>;

    TComponentContainer components; // TODO: flatmap?

public:
    virtual void reserve(size_t size)
    {
        components.reserve(size);
    }

    TComponent& add_component(EntityId id)
    {
        // "Only a single component is allowed per entity"
        assert(components.end() ==
               find_if(components.begin(), components.end(),
                       [id](auto const& component) { return std::get<0>(component) == id; }));
        auto& result = components.emplace_back();
        std::get<0>(result) = id;
        return std::get<1>(result);
    }

    TComponent* get_component(EntityId id)
    {
        auto result =
            std::find_if(components.begin(), components.end(),
                         [id](auto const& component) { return std::get<0>(component) == id; });
        if (result != components.end())
        {
            return &std::get<1>(*result);
        }
        return nullptr;
    }

    /// Erase the component belonging to the entity
    /// Returns if a component was found and removed by this id
    /// Note that when the Component destructor is called `this` will point to different memory
    /// from it's original. For this reason and more Components should not manage resources
    bool remove_component(EntityId id)
    {
        auto removed =
            std::find_if(components.begin(), components.end(),
                         [id](auto const& component) { return std::get<0>(component) == id; });
        auto result = removed != components.end();
        if (result)
        {
            components.erase(removed);
        }
        return result;
    }

    /// Apply a function to all components in the `Manager`
    /// required signature: void foo(EntityId id, TComponent& component);
    /// (the return value is ignored)
    template <typename TFun>
    void iter(TFun&& callback)
    {
        for (auto& component : components)
        {
            callback(std::get<0>(component), std::get<1>(component));
        }
    }
};
} // namespace mino

