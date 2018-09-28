#pragma once
#include "minomaly/entity.hpp"
#include <typeinfo>

namespace mino
{
using ComponentTypeId = size_t;

class Component
{
public:
    virtual ~Component();

    /// Get the component's type id
    /// Note that for polymorphic types this has 
    /// additional runtime overhead
    virtual ComponentTypeId get_type_id() const
    {
        return typeid(this).hash_code();
    };
};
} // namespace mino

