#pragma once
#include "point.hpp"
#include <array>
#include <iostream>

namespace mino
{
struct AABB
{
    Point center;
    Point radius;

    AABB() = default;
    AABB(AABB const&) = default;
    AABB(AABB&&) = default;
    ~AABB() = default;

    AABB(int x, int y, int w, int h) : center({x, y}), radius({w, h})
    {
    }

    auto intersects(AABB const& other) -> bool
    {
        return abs(center.x() - other.center.x()) <= radius.x() + other.radius.x() &&
               abs(center.y() - other.center.y()) <= radius.y() + other.radius.y();
    }

    auto contains(Point const& point) -> bool
    {
        auto min = center - radius;
        auto max = center + radius;

        for (int i = 0; i < 2; ++i)
        {
            auto const& v = point.at(i, 0);
            if ((v < min.at(i, 0)) || (v > max.at(i, 0)))
            {
                return false;
            }
        }
        return true;
    }

    auto closest(Point const& point) -> Point
    {
        Point result{};

        auto min = center - radius;
        auto max = center + radius;

        for (int i = 0; i < 2; ++i)
        {
            auto& v = result.at(i, 0);
            if (v < min.at(i, 0))
            {
                v = min.at(i, 0);
            }
            if (v > max.at(i, 0))
            {
                v = max.at(i, 0);
            }
        }
        return result;
    }
};

std::ostream& operator<<(std::ostream& os, AABB const& aabb)
{
    os << "Center:\n" << aabb.center << "\nRadius:\n" << aabb.radius << "\n";
    return os;
}

} // namespace mino

