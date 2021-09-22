#include "chimera/render/vbs/Renderable2D.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color) : pos(pos), size(size), color(color) {
    uv.push_back(glm::vec2(0.0f, 0.0f));
    uv.push_back(glm::vec2(0.0f, 1.0f));
    uv.push_back(glm::vec2(1.0f, 1.0f));
    uv.push_back(glm::vec2(1.0f, 0.0f));
    texture = nullptr;
}

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec2 size, Texture* texture) : pos(pos), size(size), texture(texture) {
    uv.push_back(glm::vec2(0.0f, 0.0f));
    uv.push_back(glm::vec2(0.0f, 1.0f));
    uv.push_back(glm::vec2(1.0f, 1.0f));
    uv.push_back(glm::vec2(1.0f, 0.0f));
}

Renderable2D::~Renderable2D() {}

} // namespace Chimera