#include "minomaly/render_system.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "minomaly/minomaly.hpp"
#include "minomaly/render_component.hpp"
#include <string>

using namespace mino;

RenderSystem::RenderSystem(SDL_Window& window,
                           SDL_Surface& surface,
                           Logger* logger,
                           std::vector<Entity>& entities,
                           Manager<RenderComponent>& render_component_manager,
                           Manager<PositionComponent>& position_component_manager)
    // TODO: pass in as params
    : renderer(
          SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    , surface(&surface)
    , logger(logger)
    , render_component_manager(render_component_manager)
    , position_component_manager(position_component_manager)
    , entities(entities)
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
    logger->debug("Destroying SDL_Textures");
    for (auto& i : textures)
    {
        SDL_DestroyTexture(i.second);
    }
    logger->debug("Destroying SDL_Renderer");
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
        SDL_RenderCopy(
            renderer, render_component.texture, &render_component.source, &render_component.dest);
    });
    SDL_RenderPresent(renderer);
    logger->debug("RenderSystem update finished");
}

RenderableEntity RenderSystem::create_renderable_entity(Entity const& entity)
{
    auto& render = render_component_manager.add_component(entity.id);
    auto& position = position_component_manager.add_component(entity.id);
    return {render, position};
}

void RenderSystem::reserve(size_t n)
{
    render_component_manager.reserve(n);
    position_component_manager.reserve(n);
}

SDL_Texture* RenderSystem::load_texture(std::string const& name)
{
    auto result = textures.find(name);
    if (result != textures.end())
    {
        return result->second;
    }
    auto media = IMG_Load(name.c_str());
    if (media == nullptr)
    {
        logger->error("Unable to load image [{}]\nError: {}", name, IMG_GetError());
        return nullptr;
    }
    auto texture = SDL_CreateTextureFromSurface(renderer, media);
    if (texture == nullptr)
    {
        logger->error("Unable to optimize image!\nSDL Error: {}", SDL_GetError());
    }
    textures.insert(std::make_pair(name, texture));
    return texture;
}

