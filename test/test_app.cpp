#include "minomaly/minomaly.hpp"
#include <cassert>
#include <iostream>

struct ExampleComponent
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
    mino::Minomaly& engine;
    mino::Manager<ExampleComponent>* component_manager;

    uint8_t updates = 0;

public:
    ExampleSystem(mino::Minomaly& engine, mino::Manager<ExampleComponent>* component_manager)
        : engine(engine)
        , component_manager(component_manager)
    {
    }
    virtual ~ExampleSystem()
    {
    }

    void start()
    {
        std::cout << "Starting ExampleSystem\n";
        for (int i = 0; i < 5; ++i)
        {
            auto entity = engine.add_entity();
            component_manager->add_component(entity.id).value = i * 100;
        }
    }

    void update()
    {
        std::cout << "-----Updating ExampleSystem-----\n";
        if (updates == 2)
        {
            bool deleted = false;
            component_manager->iter([&deleted, this](auto entity_id, auto& component) {
                if (!deleted)
                {
                    std::cout << "Removing entity by id [" << entity_id << "]\n";
                    engine.remove_entity(entity_id);
                    deleted = true;
                }
            });
        }
        else if (updates == 4)
        {
            std::cout << "Stop the engine from the ExampleSystem\n";
            engine.stop();
        }
        else
        {
            component_manager->iter([](auto entity_id, auto& component) {
                auto old = component.value;
                auto current = ++component.value;
                std::cout << "Updating component of entity [" << entity_id << "]\n"
                          << "Old value: " << old << " new value: " << current << "\n";
            });
        }

        ++updates;
    }
};

int main()
{
    // Initialize
    auto engine = mino::Minomaly();
    /* auto manager = */ engine.create_component_manager<ExampleComponent>();
    auto manager = engine.get_manager<mino::Manager<ExampleComponent>>();
    assert(manager != nullptr);
    manager->reserve(5);
    /* auto system = */ engine.create_system<ExampleSystem>(engine, manager);
    auto system = engine.get_system<ExampleSystem>();
    assert(system != nullptr);

    engine.run();
}

