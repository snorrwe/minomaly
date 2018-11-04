#pragma once
#include "matrix.hpp"
namespace mino
{

struct Point : public mino::Matrix<2, 1, int>
{
    using mino::Matrix<2, 1, int>::Matrix;

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

