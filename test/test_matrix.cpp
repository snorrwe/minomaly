#include "minomaly/common/matrix.hpp"
#include "gtest/gtest.h"

namespace
{
using namespace mino;

TEST(Matrix, CanCreateInConstexprContext)
{
    constexpr auto matrix = Matrix<2, 3>{};
    EXPECT_EQ(matrix.columns(), 2);
    EXPECT_EQ(matrix.rows(), 3);
}

TEST(Matrix, CanAccessRightElements)
{
    /*
     | 0  1  2 |
     | 3  4  5 |
    */
    auto m = Matrix<2, 3>{{0, 1, 2, 3, 4, 5}};

    EXPECT_EQ(m.at(0, 0), 0);
    EXPECT_EQ(m.at(0, 1), 1);
    EXPECT_EQ(m.at(0, 2), 2);
    EXPECT_EQ(m.at(1, 0), 3);
    EXPECT_EQ(m.at(1, 1), 4);
    EXPECT_EQ(m.at(1, 2), 5);
}

TEST(Matrix, Addition)
{
    auto m1 = Matrix<2, 3>{{0, 1, 2, 3, 4, 5}};
    auto m2 = Matrix<2, 3>{{5, 4, 3, 2, 1, 0}};

    auto result = m1 + m2;

    for (auto i = result.begin(); i != result.end(); ++i)
    {
        EXPECT_EQ(*i, 5);
    }
}

TEST(Matrix, Subtraction)
{
    auto m = Matrix<2, 3>{{0, 1, 2, 3, 4, 5}};

    auto result = m - m;

    for (auto i = result.begin(); i != result.end(); ++i)
    {
        EXPECT_EQ(*i, 0);
    }
}

TEST(Matrix, SimpleDot)
{
    auto m1 = Matrix<2, 3>{{0, 1, 2, 3, 4, 5}};
    auto m2 = Matrix<3, 4>{{0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5}};

    auto result = m1.dot(m2);

    auto expected = Matrix<2, 4>{{8, 11, 8, 11, 26, 38, 26, 38}};

    ASSERT_EQ(result, expected);
}
} // namespace

