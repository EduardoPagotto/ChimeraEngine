#pragma once
#include "chimera/render/2d/Tile.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ce {

struct SpriteComponent {
    glm::vec4 color;
    SpriteComponent(const glm::vec4& color) : color(color) {} // 15:07
    SpriteComponent(const SpriteComponent& other) = default;
};

struct Renderable3dComponent {
    IRenderable3d* renderable;
};

struct RenderableParticlesComponent {
    IRenderable3d* renderable;
    bool enable;
};

struct ComponentTile {
    TagComponent tag;
    Tile* tile = nullptr;
    ComponentTile() = default;
};

} // namespace ce
