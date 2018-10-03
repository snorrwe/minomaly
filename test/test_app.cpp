#include "minomaly/manager.hpp"
#include "minomaly/minomaly.hpp"
#include "minomaly/render_system.hpp"
#include <cassert>

using namespace mino;

constexpr size_t N_COMPONENTS = 100000;

class ExampleSystem final : public ISystem
{
    Minomaly& engine;
    Logger* logger;

    uint8_t updates = 0;

public:
    explicit ExampleSystem(Minomaly& engine)
        : engine(engine)
        , logger(engine.get_log_manager()->get_logger("example app"))
    {
        logger->debug("Created ExampleSystem");
    }

    virtual ~ExampleSystem()
    {
        logger->debug("Destroying ExampleSystem");
    }

    ExampleSystem(ExampleSystem&) = delete;
    ExampleSystem& operator=(ExampleSystem&) = delete;

    virtual void start() override
    {
        logger->info("Starting ExampleSystem");
        logger->info("Creating {} components", N_COMPONENTS);
        engine.get_manager<Manager<RenderComponent>>()->reserve(N_COMPONENTS);
        auto render_system = engine.get_system<RenderSystem>();
        for (auto i = 1; i <= N_COMPONENTS; ++i)
        {
            auto entity = engine.add_entity();

            auto components = render_system->create_renderable_entity(entity);
            components.position.x = 80;
            components.position.y = 50;
            // TODO: render component setup, texture
        }
        logger->info("ExampleSystem has started successfully");
    }

    virtual void update() override
    {
        logger->info("Updating ExampleSystem");
        constexpr auto limit = 3200;
        if (updates == limit)
        {
            logger->info("Stopping the engine from the ExampleSystem");
            engine.stop();
        }
        ++updates;
    }
};

int main(int argc, char** argv)
{
    LogManager::set_level(LogLevel::debug);

    // Initialize
    auto engine = Minomaly();
    auto system = engine.create_system<ExampleSystem>(engine);
    assert(system != nullptr);

    engine.run();

    return 0;
}

