#include "minomaly/manager.hpp"
#include "gtest/gtest.h"

namespace
{
using namespace mino;

struct ExampleComponent
{
    size_t id;

    inline static size_t count = 0;

    static void reset()
    {
        count = 0;
    }

    ExampleComponent() : id(count++)
    {
    }
};

class ManagerTest : public testing::Test
{
protected:
    Manager<ExampleComponent> manager {};

public:
    void SetUp() override
    {
        ExampleComponent::reset();
    }
};

TEST_F(ManagerTest, CanCreate)
{
}

TEST_F(ManagerTest, CanAddComponentToEntity)
{
    auto id = EntityId{0};

    EXPECT_EQ(manager.size(), 0);

    manager.add_component(id);

    EXPECT_EQ(manager.size(), 1);
}

TEST_F(ManagerTest, CanRetrieveComponent)
{

    auto id = EntityId{0};

    manager.add_component(id);

    ASSERT_EQ(manager.size(), 1);

    auto component = manager.get_component(id);
    ASSERT_NE(component, nullptr);
}

TEST_F(ManagerTest, CanRetrieveCorrectComponentById)
{

    for (unsigned i = 0; i < 10; ++i)
    {
        manager.add_component(EntityId{i});
    }

    ASSERT_EQ(manager.size(), 10);

    auto component0 = manager.get_component(EntityId{0});
    ASSERT_NE(component0, nullptr);
    EXPECT_EQ(component0->id, 0);

    auto component3 = manager.get_component(EntityId{3});
    ASSERT_NE(component3, nullptr);
    EXPECT_EQ(component3->id, 3);

    auto component9 = manager.get_component(EntityId{9});
    ASSERT_NE(component9, nullptr);
    EXPECT_EQ(component9->id, 9);

    EXPECT_NE(component0, component3);
    EXPECT_NE(component0, component9);
    EXPECT_NE(component3, component9);
}

TEST_F(ManagerTest, GetComponentReturnsNullIfNoComponentByIdExists)
{

    for (unsigned i = 0; i < 10; ++i)
    {
        manager.add_component(EntityId{i});
    }

    ASSERT_EQ(manager.size(), 10);

    auto component = manager.get_component(EntityId{128});
    EXPECT_EQ(component, nullptr);
}

TEST_F(ManagerTest, GetOrAddComponentWorksAsExpected)
{

    manager.add_component(EntityId{0});

    ASSERT_EQ(manager.size(), 1);

    auto component = manager.get_or_add_component(EntityId{1});

    EXPECT_EQ(manager.size(), 2);
    EXPECT_EQ(component.id, 1);

    component = manager.get_or_add_component(EntityId{0});

    EXPECT_EQ(manager.size(), 2);
    EXPECT_EQ(component.id, 0);
}

TEST_F(ManagerTest, RemoveComponentWorksAsIntended)
{
    for (unsigned i = 0; i < 10; ++i)
    {
        manager.add_component(EntityId{i});
    }

    ASSERT_EQ(manager.size(), 10);

    auto component = manager.get_component(EntityId{8});
    ASSERT_NE(component, nullptr);

    manager.remove_component(EntityId{8});

    EXPECT_EQ(manager.size(), 9);

    component = manager.get_component(EntityId{8});
    ASSERT_EQ(component, nullptr);
}
}; // namespace

