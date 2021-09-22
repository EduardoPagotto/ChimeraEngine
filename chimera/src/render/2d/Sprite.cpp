#include "chimera/render/2d/Sprite.hpp"

namespace Chimera {

Sprite::Sprite(float x, float y, float width, float height, glm::vec4 color)
    : Renderable2D(glm::vec3(x, y, 0), glm::vec2(width, height), color) {}

Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
    : Renderable2D(glm::vec3(x, y, 0), glm::vec2(width, height), texture) {}

Sprite::~Sprite() {}

} // namespace Chimera