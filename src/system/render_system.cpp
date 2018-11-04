#include "minomaly/system/render_system.hpp"
#include "SDL.h"
#include "SDL_image.h"
#include "minomaly/component/render_component.hpp"
#include "minomaly/minomaly.hpp"
#include <string>

using namespace mino;

RenderSystem::RenderSystem(SDL_Window& window,
                           Logger* logger,
                           Manager<RenderComponent>& render_component_manager,
                           Manager<PositionComponent>& position_component_manager)
    // TODO: pass in as params
    : renderer(
          SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))
    , logger(logger)
    , render_component_manager(render_component_manager)
    , position_component_manager(position_component_manager)
{
    if (renderer == nullptr)
    {
        logger->error("RenderSystem could not be created! SDL Error:\n{}", SDL_GetError());
        throw std::runtime_error("RenderSystem could not be created!");
    }
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
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
    SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0x0);
    SDL_RenderClear(renderer);
    logger->debug("Draw RenderComponents");
    auto& position_components = position_component_manager.get_components();
    auto current_position = position_components.begin();
    render_component_manager.iter([&, this](auto entity, auto& render_component) {
        // Zip render and position components
        while (std::get<0>(*current_position) != entity)
        {
            ++current_position;
            assert(current_position != position_components.end());
        }
        assert(current_position != position_components.end());

        auto& position = std::get<1>(*current_position);
        render_component.dest.x = position.x();
        render_component.dest.y = position.y();
        SDL_RenderCopy(
            renderer, render_component.texture, &render_component.source, &render_component.dest);
    });
    logger->debug("RenderPresent");
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
    logger->debug("Loading image {}", name);
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
    logger->debug("Successfully loaded image {}", name);
    textures.insert(std::make_pair(name, texture));
    return texture;
}

