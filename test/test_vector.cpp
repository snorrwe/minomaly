#include "minomaly/common/vector.hpp"
#include "gtest/gtest.h"

namespace
{
using namespace mino;

TEST(Vector2, CanCalculateDotProductWithIdentityMatrix)
{
    auto v = Vector2{std::array{1.0f, 2.0f}};

    auto m = Matrix<2, 2>{std::array{1.0f, 0.0f, 0.0f, 1.0f}};

    auto result = v.dot(m);

    const auto expected = Vector2{std::array{1.0f, 2.0f}};

    ASSERT_EQ(result, expected);
}

TEST(Vector2, CanCalculateDotProductWithNullMatrix)
{
    auto v = Vector2{std::array{1.0f, 2.0f}};

    auto m = Matrix<2, 2>{std::array{0.0f, 0.0f, 0.0f, 0.0f}};

    auto result = v.dot(m);

    const auto expected = Vector2{std::array{0.0f, 0.0f}};

    ASSERT_EQ(result, expected);
}

TEST(Vector2, CanCalculateDotProductWithRotationMatrix)
{
    auto v = Vector2{std::array{1.0f, 2.0f}};

    auto m = Matrix<2, 2>{std::array{0.0f, -1.0f, 1.0f, 0.0f}};
    auto result = v.dot(m);

    const auto expected = Vector2{std::array{2.0f, -1.0f}};

    ASSERT_EQ(result, expected);
}

TEST(Vector2, CanAddTwoVectors)
{
    const auto v1 = Vector2{std::array{1.0f, 2.0f}};
    const auto v2 = Vector2{std::array{2.0f, 1.0f}};

    const auto result = v1 + v2;

    EXPECT_EQ(result.at(0), 3.0f);
    EXPECT_EQ(result.at(1), 3.0f);
}
} // namespace

