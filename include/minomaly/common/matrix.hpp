#pragma once
#include <array>
#include <cassert>

namespace mino
{
template <uint16_t _cols, uint16_t _rows, typename TValue = float>
class Matrix
{

public:
    using Self = Matrix<_cols, _rows, TValue>;
    using ValueContainer = std::array<TValue, _cols * _rows>;
    using ValuesIterator = typename ValueContainer::iterator;
    using ValuesConstIterator = typename ValueContainer::const_iterator;

    constexpr Matrix() = default;
    explicit constexpr Matrix(ValueContainer const& values) : values(values)
    {
    }
    explicit constexpr Matrix(ValueContainer&& values) : values(std::move(values))
    {
    }
    constexpr Matrix(Matrix const&) = default;
    constexpr Matrix(Matrix&&) = default;
    ~Matrix() = default;
    constexpr Matrix& operator=(Matrix const&) = default;
    constexpr Matrix& operator=(Matrix&&) = default;

    constexpr auto columns() const -> uint16_t;
    constexpr auto rows() const -> uint16_t;
    constexpr auto size() const -> size_t;

    constexpr auto at(uint16_t col, uint16_t row) -> TValue&;
    constexpr auto at(uint16_t col, uint16_t row) const -> TValue const&;

    constexpr auto begin() -> ValuesIterator;
    constexpr auto begin() const -> ValuesConstIterator;

    constexpr auto end() -> ValuesIterator;
    constexpr auto end() const -> ValuesConstIterator;

    constexpr auto operator+(Self const& other) const -> Self;
    constexpr auto operator+=(Self const& other) -> Self&;

    constexpr auto operator-(Self const& other) const -> Self;
    constexpr auto operator-=(Self const& other) -> Self&;

private:
    constexpr TValue& _at(uint16_t col, uint16_t row);

    ValueContainer values;
};

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr uint16_t Matrix<_cols, _rows, TValue>::columns() const
{
    return _cols;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr uint16_t Matrix<_cols, _rows, TValue>::rows() const
{
    return _rows;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr size_t Matrix<_cols, _rows, TValue>::size() const
{
    return _cols * _rows;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr TValue& Matrix<_cols, _rows, TValue>::at(uint16_t col, uint16_t row)
{
    return _at(col, row);
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr TValue const& Matrix<_cols, _rows, TValue>::at(uint16_t col, uint16_t row) const
{
    return _at(col, row);
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr typename Matrix<_cols, _rows, TValue>::ValuesIterator
Matrix<_cols, _rows, TValue>::begin()
{
    return values.begin();
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr typename Matrix<_cols, _rows, TValue>::ValuesConstIterator
Matrix<_cols, _rows, TValue>::begin() const
{
    return values.begin();
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr typename Matrix<_cols, _rows, TValue>::ValuesIterator Matrix<_cols, _rows, TValue>::end()
{
    return values.end();
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr typename Matrix<_cols, _rows, TValue>::ValuesConstIterator
Matrix<_cols, _rows, TValue>::end() const
{
    return values.end();
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr Matrix<_cols, _rows, TValue> Matrix<_cols, _rows, TValue>::
operator+(Matrix<_cols, _rows, TValue> const& other) const
{
    auto result = Matrix{*this};
    result += other;
    return result;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr Matrix<_cols, _rows, TValue>& Matrix<_cols, _rows, TValue>::
operator+=(Matrix<_cols, _rows, TValue> const& other)
{
    auto i = this->values.begin();
    auto j = other.values.begin();
    for (; i != values.end(); ++i, ++j)
    {
        *i += *j;
    }
    return *this;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr Matrix<_cols, _rows, TValue> Matrix<_cols, _rows, TValue>::
operator-(Matrix<_cols, _rows, TValue> const& other) const
{
    auto result = Matrix{*this};
    result -= other;
    return result;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr Matrix<_cols, _rows, TValue>& Matrix<_cols, _rows, TValue>::
operator-=(Matrix<_cols, _rows, TValue> const& other)
{
    auto i = this->values.begin();
    auto j = other.values.begin();
    for (; i != values.end(); ++i, ++j)
    {
        *i -= *j;
    }
    return *this;
}

template <uint16_t _cols, uint16_t _rows, typename TValue>
constexpr TValue& Matrix<_cols, _rows, TValue>::_at(uint16_t col, uint16_t row)
{
    assert(col < columns());
    assert(row < rows());
    return values[col + row * columns()];
}
} // namespace mino

