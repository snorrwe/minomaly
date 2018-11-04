#pragma once
#include "matrix.hpp"
#include <memory>

namespace mino
{
/// Vectors are matrices with a single row
template <size_t dim, typename TValue = float>
struct Vector : public Matrix<1, dim, TValue>
{
    using Matrix<1, dim, TValue>::Matrix;
    Vector(Matrix<1, dim, TValue> const& m) : Matrix<1, dim, TValue>(m)
    {
    }
    Vector(Matrix<1, dim, TValue>&& m) : Matrix<1, dim, TValue>(std::move(m))
    {
    }

    constexpr auto at(size_t x) -> TValue&
    {
        return Matrix<1, dim, TValue>::at(0, x);
    }

    constexpr auto at(size_t x) const -> TValue const&
    {
        return Matrix<1, dim, TValue>::at(0, x);
    }

    constexpr auto operator+(Vector const& other) const -> Vector const
    {
        return static_cast<Vector>(Matrix<1, dim, TValue>::operator+(other));
    }

    constexpr auto operator+=(Vector const& other) -> Vector&
    {
        return Matrix<1, dim, TValue>::operator+=(other);
    }

    constexpr auto operator-(Vector const& other) const -> Vector const
    {
        return static_cast<Vector>(Matrix<1, dim, TValue>::operator-(other));
    }

    constexpr auto operator-=(Vector const& other) -> Vector&
    {
        return Matrix<1, dim, TValue>::operator-=(other);
    }
};

using Vector2 = Vector<2, float>;
using Vector3 = Vector<3, float>;
} // namespace mino

