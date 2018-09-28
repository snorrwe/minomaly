#include "minomaly/minomaly.hpp"
#include <iostream>

struct ExampleComponent final
{
    size_t value;

    ExampleComponent()
    {
        std::cout << "Creating an example component\n";
    }

    ~ExampleComponent()
    {
        std::cout << "Destroying an example component. Value: " << value << "\n";
    }
};

class ExampleSystem final : public mino::ISystem
{
    mino::Manager<ExampleComponent>* component_manager;

public:
    ExampleSystem(mino::Manager<ExampleComponent>* component_manager)
        : component_manager(component_manager)
    {
    }
    virtual ~ExampleSystem()
    {
    }

    void update()
    {
        std::cout << "Updating ExampleSystem\n";

        component_manager->iter([](auto entity_id, auto& component) {
            auto old = component.value;
            auto current = ++component.value;
            std::cout << "Updating component of entity [" << entity_id << "]\n"
                      << "Old id: " << old << " new id: " << current << "\n";
        });
    }
};

int main()
{
    // Initialize
    auto engine = mino::Minomaly();
    auto manager = engine.create_component_manager<ExampleComponent>();
    auto system = engine.create_system<ExampleSystem>(manager);

    manager->reserve(5);

    // Create some components
    auto entity = engine.add_entity();
    manager->add_component(entity.id).value = 1;

    auto other_entity = engine.add_entity();
    manager->add_component(other_entity.id);
    manager->get_component(other_entity.id)->value = 2;

    // Update the application
    engine.update();

    // Remove one of the components
    std::cout << "Remove entity [" << other_entity.id << "]\n";
    engine.remove_entity(other_entity.id);

    // Update the application
    for (int i = 0; i < 2; ++i)
    {
        engine.update();
    }
}

