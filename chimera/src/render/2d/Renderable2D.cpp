#include "chimera/render/2d/Renderable2D.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Renderable2D::Renderable2D(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {

    prop2d.position = pos;
    prop2d.size = size;
    prop2d.color = color;
    prop2d.texture = nullptr;
    prop2d.uv = Renderable2D::getDefaultUVs();
}

Renderable2D::Renderable2D(const glm::vec3& pos, const glm::vec2& size, Texture* texture) {

    prop2d.position = pos;
    prop2d.size = size;
    prop2d.color = glm::vec4(1, 1, 1, 1);
    prop2d.texture = texture;
    prop2d.uv = Renderable2D::getDefaultUVs();
}

const std::vector<glm::vec2>& Renderable2D::getDefaultUVs() {
    static std::vector<glm::vec2> results;
    if (!results.size()) {
        results.push_back(glm::vec2(0, 1));
        results.push_back(glm::vec2(1, 1));
        results.push_back(glm::vec2(1, 0));
        results.push_back(glm::vec2(0, 0));
    }
    return results;
}
} // namespace Chimera