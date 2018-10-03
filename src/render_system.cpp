#include "minomaly/render_system.hpp"
#include "SDL.h"
#include "minomaly/minomaly.hpp"
#include "minomaly/render_component.hpp"

using namespace mino;

RenderSystem::RenderSystem(SDL_Window& window, Logger* logger, std::vector<Entity>& entities,
                           Manager<RenderComponent>& render_component_manager,
                           Manager<PositionComponent>& position_component_manager)
    // TODO: pass in as params
    : renderer(
          SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    , logger(logger)
    , entities(entities)
    , render_component_manager(render_component_manager)
    , position_component_manager(position_component_manager)
{
    if (renderer == nullptr)
    {
        logger->error("RenderSystem could not be created! SDL Error:\n{}", SDL_GetError());
        throw std::runtime_error("RenderSystem could not be created!");
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    logger->debug("RenderSystem was initialized successfully");
}

RenderSystem::~RenderSystem()
{
    logger->debug("Destroying RenderSystem");
    SDL_DestroyRenderer(renderer);
}

void RenderSystem::update()
{
    logger->debug("RenderSystem update starting...");
    SDL_RenderClear(renderer);
    render_component_manager.iter([this](auto entity, auto& render_component) {
        auto position = position_component_manager.get_component(entity);
        assert(position != nullptr);
        render_component.dest.x = position->x;
        render_component.dest.y = position->y;
        SDL_RenderCopy(renderer, render_component.texture, &render_component.source,
                       &render_component.dest);
    });
    SDL_RenderPresent(renderer);
    logger->debug("RenderSystem update finished");
}

RenderableEntity RenderSystem::create_renderable_entity(Entity& entity)
{
    auto render = render_component_manager.add_component(entity.id);
    auto position = position_component_manager.add_component(entity.id);
    return {render, position};
}

