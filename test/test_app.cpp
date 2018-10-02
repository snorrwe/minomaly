#include "minomaly/minomaly.hpp"
#include <cassert>
#include <iostream>

struct ExampleComponent
{
    size_t value;
};

constexpr size_t N_COMPONENTS = 100;

class ExampleSystem final : public mino::ISystem
{
    mino::Minomaly& engine;
    mino::Manager<ExampleComponent>* component_manager;
    mino::Logger* logger;

    uint8_t updates = 0;

public:
    ExampleSystem(mino::Minomaly& engine)
        : engine(engine)
        , component_manager(engine.create_component_manager<ExampleComponent>())
        , logger(engine.get_log_manager()->get_logger("example app"))
    {
        assert(component_manager);
        logger->debug("Created ExampleSystem");
    }

    virtual ~ExampleSystem()
    {
        logger->debug("Destroying ExampleSystem");
    }

    virtual void start() override
    {
        logger->info("Starting ExampleSystem");
        logger->info("Creating {} components", N_COMPONENTS);
        component_manager->reserve(N_COMPONENTS);
        for (auto i = 1; i <= N_COMPONENTS; ++i)
        {
            auto entity = engine.add_entity();
            component_manager->add_component(entity.id).value = i * N_COMPONENTS;
        }
        logger->info("ExampleSystem has started successfully");
    }

    virtual void update() override
    {
        logger->info("-----Updating ExampleSystem-----");
        if (updates == 2)
        {
            bool deleted = false;
            component_manager->iter([&deleted, this](auto entity_id, auto& component) {
                if (!deleted)
                {
                    logger->info("Removing entity by id [{:#06x}]", entity_id);
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
    auto system = engine.create_system<ExampleSystem>(engine);
    assert(system != nullptr);

    engine.run();
}

