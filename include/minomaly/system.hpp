#pragma once

namespace mino
{
class ISystem
{
public:
    virtual ~ISystem()
    {
    }
    virtual void update() = 0;
};
} // namespace mino

