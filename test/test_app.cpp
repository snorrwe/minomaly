#include "minomaly/minomaly.hpp"
#include <cassert>
#include <iostream>

struct ExampleComponent
{
    size_t value;
};

class ExampleSystem final : public mino::ISystem
{
    mino::Minomaly& engine;
    mino::Manager<ExampleComponent>* component_manager;
    mino::Logger* logger;

    uint8_t updates = 0;

public:
    ExampleSystem(mino::Minomaly& engine, mino::Manager<ExampleComponent>* component_manager)
        : engine(engine)
        , component_manager(component_manager)
        , logger(engine.get_log_manager()->get_logger("example app"))
    {
        logger->debug("Created ExampleSystem");
    }
    virtual ~ExampleSystem()
    {
        logger->debug("Destroying ExampleSystem");
    }

    void start()
    {
        logger->info("Starting ExampleSystem");
        for (int i = 1; i <= 5; ++i)
        {
            auto entity = engine.add_entity();
            component_manager->add_component(entity.id).value = i * 100;
        }
    }

    void update()
    {
        logger->info("-----Updating ExampleSystem-----");
        if (updates == 2)
        {
            bool deleted = false;
            component_manager->iter([&deleted, this](auto entity_id, auto& component) {
                if (!deleted)
                {
                    logger->info("Removing entity by id [{}]", entity_id);
                    engine.remove_entity(entity_id);
                    deleted = true;
                }
            });
        }
        else if (updates == 4)
        {
            logger->info("Stopping the engine from the ExampleSystem");
            engine.stop();
        }
        else
        {
            component_manager->iter([this](auto entity_id, auto& component) {
                auto old = component.value;
                auto current = ++component.value;
                logger->info("Updating component of entity [{}] Old value: [{}] new value: [{}]",
                             entity_id, old, current);
            });
        }

        ++updates;
    }
};

int main()
{
    mino::LogManager::set_level(mino::LogLevel::debug);

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

