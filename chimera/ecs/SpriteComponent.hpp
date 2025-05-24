#pragma once
#include "ecs.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace ce {
    struct SpriteComponent {
        TagInfo tag;
        glm::vec4 color;
        SpriteComponent(const glm::vec4& color) : color(color) {} // 15:07
        SpriteComponent(const SpriteComponent& other) = default;
    };
} // namespace ce
