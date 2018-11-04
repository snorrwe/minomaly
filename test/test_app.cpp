#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "minomaly/manager.hpp"
#include "minomaly/minomaly.hpp"
#include "minomaly/system/render_system.hpp"
#include <cassert>
#include <random>

using namespace mino;

constexpr size_t N_COMPONENTS = 1000 * 100;
constexpr size_t WORLD_W = 800;
constexpr size_t WORLD_H = 600;

class ExampleSystem final : public ISystem
{
    Minomaly& engine;
    Manager<PositionComponent>* position_component_manager = nullptr;
    Logger* logger = nullptr;

    uint8_t updates = 0;
    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist_w;
    std::uniform_int_distribution<std::mt19937::result_type> dist_h;

public:
    explicit ExampleSystem(Minomaly& engine)
        : engine(engine)
        , logger(engine.get_log_manager()->get_logger("example app"))
        , dist_w(0, WORLD_W)
        , dist_h(0, WORLD_H)
    {
        rng.seed(std::random_device()());
        logger->debug("Created ExampleSystem");
    }

    ~ExampleSystem() override
    {
        logger->debug("Destroying ExampleSystem");
    }

    ExampleSystem(ExampleSystem const&) = delete;
    ExampleSystem& operator=(ExampleSystem const&) = delete;

    void start() override
    {
        logger->info("Starting ExampleSystem");

        position_component_manager = engine.get_or_create_manager<Manager<PositionComponent>>();
        assert(position_component_manager != nullptr);

        auto render_system = engine.get_system<RenderSystem>();
        render_system->reserve(N_COMPONENTS);
        auto texture = render_system->load_texture(".data/boii.png");

        logger->info("Creating renderable entities + garbage entities");
        auto count = 0;
        for (size_t i = 0; i < N_COMPONENTS; ++i)
        {
            auto const& entity = engine.add_entity();
            auto s = dist_w(rng);
            if ((s & 1) == 0)
            {
                // Roughly every second entity is renderable
                ++count;
                auto components = render_system->create_renderable_entity(entity);
                components.position.x() = 0;
                components.position.y() = 0;
                components.render.texture = texture;
                components.render.source.x = 0;
                components.render.source.y = 0;
                components.render.source.w = 20;
                components.render.source.h = 20;
                components.render.dest.w = 20;
                components.render.dest.h = 20;
            }
        }
        logger->info("Created {} renderable and {} garbage entities", count, N_COMPONENTS - count);
        logger->info("ExampleSystem has started successfully");
    }

    void update() override
    {
        logger->info("Updating ExampleSystem");
        constexpr auto limit = 32;
        if (updates == limit)
        {
            logger->info("Stopping the engine from the ExampleSystem");
            engine.stop();
        }
        else
        {
            logger->info("Randomizing the positions of components");
            position_component_manager->iter([&](auto, auto& position) {
                position.x() = dist_w(rng);
                position.y() = dist_h(rng);
            });
        }
        ++updates;
        logger->info("ExampleSystem was updated successfully");
    }
};

int main(int, char**)
{
    LogManager::set_level(LogLevel::debug);

    // Initialize
    auto engine = Minomaly();
    engine.create_system<ExampleSystem>(engine);

    engine.run();

    return 0;
}

