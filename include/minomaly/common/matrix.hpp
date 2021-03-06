#pragma once
#include <array>
#include <cassert>
#include <iostream>

namespace mino
{
template <size_t _rows, size_t _cols, typename TValue = float>
class Matrix
{

public:
    using Self = Matrix<_rows, _cols, TValue>;
    using ValueContainer = std::array<TValue, _rows * _cols>;
    using ValuesIterator = typename ValueContainer::iterator;
    using ValuesConstIterator = typename ValueContainer::const_iterator;

    constexpr Matrix() = default;
    constexpr Matrix(ValueContainer const& values) : values(values)
    {
    }
    constexpr Matrix(ValueContainer&& values) : values(std::move(values))
    {
    }
    constexpr Matrix(Matrix const&) = default;
    constexpr Matrix(Matrix&&) = default;
    ~Matrix() = default;
    constexpr Matrix& operator=(Matrix const&) = default;
    constexpr Matrix& operator=(Matrix&&) = default;

    constexpr auto columns() const -> size_t;
    constexpr auto rows() const -> size_t;
    constexpr auto size() const -> size_t;

    constexpr auto at(size_t col, size_t row) -> TValue&;
    constexpr auto at(size_t col, size_t row) const -> TValue const&;

    constexpr auto begin() -> ValuesIterator;
    constexpr auto begin() const -> ValuesConstIterator;

    constexpr auto end() -> ValuesIterator;
    constexpr auto end() const -> ValuesConstIterator;

    template <size_t other_rows>
    constexpr auto dot(Matrix<_cols, other_rows, TValue> const& other) const
        -> Matrix<_rows, other_rows, TValue>;
    template <size_t other_rows>
    constexpr auto dot(Matrix<_cols, other_rows, TValue> const& other,
                       Matrix<_rows, other_rows, TValue>& out) const -> void;

    constexpr auto operator+(Self const& other) const -> Self;
    constexpr auto operator+=(Self const& other) -> Self&;

    constexpr auto operator-(Self const& other) const -> Self;
    constexpr auto operator-=(Self const& other) -> Self&;

    constexpr auto operator==(Self const& other) const -> bool;

    template <typename TOther = TValue>
    constexpr auto operator*(TOther other) const -> Self;
    template <typename TOther = TValue>
    constexpr auto operator*=(TOther other) -> Self&;
    template <typename TOther = TValue>
    constexpr auto operator/(TOther other) const -> Self;
    template <typename TOther = TValue>
    constexpr auto operator/=(TOther other) -> Self&;

private:
    ValueContainer values;
};

template <size_t _rows, size_t _cols, typename TValue>
constexpr size_t Matrix<_rows, _cols, TValue>::columns() const
{
    return _cols;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr size_t Matrix<_rows, _cols, TValue>::rows() const
{
    return _rows;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr size_t Matrix<_rows, _cols, TValue>::size() const
{
    return _rows * _cols;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr TValue& Matrix<_rows, _cols, TValue>::at(size_t row, size_t col)
{
    assert(col < columns());
    assert(row < rows());
    return values[col + row * columns()];
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr TValue const& Matrix<_rows, _cols, TValue>::at(size_t row, size_t col) const
{
    assert(col < columns());
    assert(row < rows());
    return values[col + row * columns()];
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr typename Matrix<_rows, _cols, TValue>::ValuesIterator
Matrix<_rows, _cols, TValue>::begin()
{
    return values.begin();
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr typename Matrix<_rows, _cols, TValue>::ValuesConstIterator
Matrix<_rows, _cols, TValue>::begin() const
{
    return values.begin();
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr typename Matrix<_rows, _cols, TValue>::ValuesIterator Matrix<_rows, _cols, TValue>::end()
{
    return values.end();
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr typename Matrix<_rows, _cols, TValue>::ValuesConstIterator
Matrix<_rows, _cols, TValue>::end() const
{
    return values.end();
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr Matrix<_rows, _cols, TValue> Matrix<_rows, _cols, TValue>::
operator+(Matrix<_rows, _cols, TValue> const& other) const
{
    auto result = Matrix{*this};
    result += other;
    return result;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr Matrix<_rows, _cols, TValue>& Matrix<_rows, _cols, TValue>::
operator+=(Matrix<_rows, _cols, TValue> const& other)
{
    auto i = this->values.begin();
    auto j = other.values.begin();
    for (; i != values.end(); ++i, ++j)
    {
        *i += *j;
    }
    return *this;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr Matrix<_rows, _cols, TValue> Matrix<_rows, _cols, TValue>::
operator-(Matrix<_rows, _cols, TValue> const& other) const
{
    auto result = Matrix{*this};
    result -= other;
    return result;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr Matrix<_rows, _cols, TValue>& Matrix<_rows, _cols, TValue>::
operator-=(Matrix<_rows, _cols, TValue> const& other)
{
    auto i = this->values.begin();
    auto j = other.values.begin();
    for (; i != values.end(); ++i, ++j)
    {
        *i -= *j;
    }
    return *this;
}

template <size_t _rows, size_t _cols, typename TValue>
template <size_t other_rows>
constexpr void Matrix<_rows, _cols, TValue>::dot(Matrix<_cols, other_rows, TValue> const& other,
                                                 Matrix<_rows, other_rows, TValue>& out) const
{
    for (auto r = 0; r < _rows; ++r)
    {
        for (auto c = 0; c < other_rows; ++c)
        {
            out.at(r, c) = 0;
            for (auto k = 0; k < _cols; ++k)
            {
                out.at(r, c) += at(r, k) * other.at(k, c);
            }
        }
    }
}

template <size_t _rows, size_t _cols, typename TValue>
template <size_t other_rows>
constexpr Matrix<_rows, other_rows, TValue>
Matrix<_rows, _cols, TValue>::dot(Matrix<_cols, other_rows, TValue> const& other) const
{
    Matrix<_rows, other_rows> result{};
    dot(other, result);
    return result;
}

template <size_t _rows, size_t _cols, typename TValue>
constexpr auto Matrix<_rows, _cols, TValue>::operator==(Self const& other) const -> bool
{
    auto i = this->begin();
    auto j = other.begin();
    for (; i != this->end(); ++i, ++j)
    {
        if (*i != *j)
        {
            return false;
        }
    }
    return true;
}

template <size_t _rows, size_t _cols, typename TValue>
std::ostream& operator<<(std::ostream& os, Matrix<_rows, _cols, TValue> const& matrix)
{
    for (auto r = 0; r < _rows; ++r)
    {
        os << matrix.at(r, 0);
        for (auto c = 1; c < _cols; ++c)
        {
            os << ' ' << matrix.at(r, c);
        }
        os << '\n';
    }
    return os;
}

template <size_t _rows, size_t _cols, typename TValue>
template <typename TOther>
constexpr auto Matrix<_rows, _cols, TValue>::operator*(TOther other) const -> Self
{
    Self result{*this};
    result *= other;
    return result;
}

template <size_t _rows, size_t _cols, typename TValue>
template <typename TOther>
constexpr auto Matrix<_rows, _cols, TValue>::operator*=(TOther other) -> Self&
{
    for (auto& v : values)
    {
        v *= other;
    }
    return *this;
}

template <size_t _rows, size_t _cols, typename TValue>
template <typename TOther>
constexpr auto Matrix<_rows, _cols, TValue>::operator/(TOther other) const -> Self
{
    Self result{*this};
    result /= other;
    return result;
}

template <size_t _rows, size_t _cols, typename TValue>
template <typename TOther>
constexpr auto Matrix<_rows, _cols, TValue>::operator/=(TOther other) -> Self&
{
    for (auto& v : values)
    {
        v /= other;
    }
    return *this;
}

} // namespace mino

