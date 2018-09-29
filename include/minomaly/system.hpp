#pragma once

namespace mino
{
class ISystem
{
public:
    virtual ~ISystem()
    {
    }

    virtual void start()
    {
    }
    virtual void update()
    {
    }
};
} // namespace mino

