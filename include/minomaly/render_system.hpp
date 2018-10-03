#pragma once
#include "entity.hpp"
#include "logger.hpp"
#include "manager.hpp"
#include "position_component.hpp"
#include "render_component.hpp"
#include "system.hpp"
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

namespace mino
{

class Minomaly;

struct RenderableEntity
{
    RenderComponent& render;
    PositionComponent& position;
};

class RenderSystem final : public ISystem
{
    SDL_Renderer* renderer;
    Logger* logger;
    Manager<RenderComponent>& render_component_manager;
    Manager<PositionComponent>& position_component_manager;
    std::vector<Entity>& entities;

public:
    RenderSystem(SDL_Window& window, Logger* logger, std::vector<Entity>& entities,
                 Manager<RenderComponent>&, Manager<PositionComponent>&);
    virtual ~RenderSystem();

    virtual void update() override;
    RenderableEntity create_renderable_entity(Entity& entity);

    // TODO: 
    // load_texture
    // load_spritesheet
    //
    // TextureManager
};
} // namespace mino

