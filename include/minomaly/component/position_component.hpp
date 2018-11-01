#pragma once

namespace mino
{
struct PositionComponent
{
    int x;
    int y;

    constexpr auto operator+(PositionComponent const& other) -> PositionComponent
    {
        auto result = PositionComponent{*this};
        result += other;
        return result;
    }

    constexpr auto operator+=(PositionComponent const& other) -> PositionComponent&
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr auto operator-(PositionComponent const& other) -> PositionComponent
    {
        auto result = PositionComponent{*this};
        result -= other;
        return result;
    }

    constexpr auto operator-=(PositionComponent const& other) -> PositionComponent&
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};
} // namespace mino

