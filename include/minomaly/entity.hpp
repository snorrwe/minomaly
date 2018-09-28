#pragma once

namespace mino
{

using EntityId = size_t;

/// Represents properties shared between entities
struct Entity
{
    EntityId id;
    size_t flag;
};

} // namespace mino

