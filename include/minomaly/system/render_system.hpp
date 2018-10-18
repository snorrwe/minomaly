#pragma once
#include "minomaly/component/position_component.hpp"
#include "minomaly/component/render_component.hpp"
#include "minomaly/entity.hpp"
#include "minomaly/logger.hpp"
#include "minomaly/manager.hpp"
#include "system.hpp"
#include <iosfwd>
#include <unordered_map>
#include <vector>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Texture;

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
    std::unordered_map<std::string, SDL_Texture*> textures;

public:
    RenderSystem(SDL_Window& window,
                 Logger* logger,
                 Manager<RenderComponent>&,
                 Manager<PositionComponent>&);
    ~RenderSystem() override;

    RenderSystem(RenderSystem const&) = delete;
    RenderSystem(RenderSystem&&) = delete;
    RenderSystem& operator=(RenderSystem const&) = delete;
    RenderSystem& operator=(RenderSystem&&) = delete;

    void update() override;

    /// Reserve capacity for n >= 0 renderable entities
    void reserve(size_t n);
    RenderableEntity create_renderable_entity(Entity const& entity);

    SDL_Texture* load_texture(std::string const& name);
    // TODO:
    // load_spritesheet
    // TextureManager
};
} // namespace mino

