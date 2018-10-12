#pragma once
#include "entity.hpp"
#include "logger.hpp"
#include "manager.hpp"
#include "position_component.hpp"
#include "render_component.hpp"
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
    SDL_Surface* surface;
    Logger* logger;
    Manager<RenderComponent>& render_component_manager;
    Manager<PositionComponent>& position_component_manager;
    std::vector<Entity>& entities;
    std::unordered_map<std::string, SDL_Texture*> textures;

public:
    RenderSystem(SDL_Window& window,
                 SDL_Surface& surface,
                 Logger* logger,
                 std::vector<Entity>& entities,
                 Manager<RenderComponent>&,
                 Manager<PositionComponent>&);
    ~RenderSystem() override;

    RenderSystem(RenderSystem const&) = delete;
    RenderSystem(RenderSystem &&) = delete;
    RenderSystem& operator=(RenderSystem const&) = delete;
    RenderSystem& operator=(RenderSystem &&) = delete;

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

