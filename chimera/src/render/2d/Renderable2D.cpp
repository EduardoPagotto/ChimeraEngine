#include "chimera/render/2d/Renderable2D.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Chimera {

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec2 size, glm::vec4 color) : pos(pos), size(size), color(color), texture(nullptr) {
    uv = Renderable2D::getDefaultUVs();
}

Renderable2D::Renderable2D(glm::vec3 pos, glm::vec2 size, Texture* texture)
    : pos(pos), size(size), texture(texture), color(glm::vec4(1, 1, 1, 1)) {
    uv = Renderable2D::getDefaultUVs();
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