#pragma once
#include "matrix.hpp"
namespace mino
{

struct Point : public mino::Matrix<2, 1, int>
{
    using mino::Matrix<2, 1, int>::Matrix;
    using mino::Matrix<2, 1, int>::operator*=;
    using mino::Matrix<2, 1, int>::operator*;
    using mino::Matrix<2, 1, int>::operator/=;
    using mino::Matrix<2, 1, int>::operator/;
    using mino::Matrix<2, 1, int>::operator=;

    constexpr Point(mino::Matrix<2, 1, int> const& m) : mino::Matrix<2, 1, int>(m)
    {
    }

    constexpr Point(int x, int y) : mino::Matrix<2, 1, int>({x, y})
    {
    }

    int& x()
    {
        return at(0, 0);
    }

    const int x() const
    {
        return at(0, 0);
    }

    int& y()
    {
        return at(1, 0);
    }

    const int y() const
    {
        return at(1, 0);
    }
};

} // namespace mino

