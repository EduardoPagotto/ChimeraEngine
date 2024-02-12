#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Chimera {
struct Prop2D {
    Prop2D() = default;
    glm::vec3 position = glm::vec3(0.0);
    glm::vec2 size = glm::vec2(1.0);
    glm::vec4 color = glm::vec4(1.0);
    class Texture* texture = nullptr;
    std::vector<glm::vec2> uv;
};
} // namespace Chimera