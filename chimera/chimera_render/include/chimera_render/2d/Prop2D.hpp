#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace ce {

    struct Prop2D {
        Prop2D() = default;
        glm::vec3 position = glm::vec3(0.0);
        glm::vec2 size = glm::vec2(1.0);
        glm::vec4 color = glm::vec4(1.0);
        std::shared_ptr<class Texture> texture;
        std::vector<glm::vec2> uv;
    };
} // namespace ce
