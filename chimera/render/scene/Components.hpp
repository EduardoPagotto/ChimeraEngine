#ifndef __CHIMERA_COMPONENTS__HPP
#define __CHIMERA_COMPONENTS__HPP

#include <glm/glm.hpp>

namespace Chimera {

struct TagComponent {
    std::string tag;
    TagComponent() = default;
    TagComponent(const TagComponent& tag) = default;
    TagComponent(const std::string& tag) : tag(tag) {}
};

struct SpriteComponent {
    glm::vec4 color;
    SpriteComponent(const glm::vec4& color) : color(color) {} // 15:07
    SpriteComponent(const SpriteComponent& other) = default;
};

struct TransformComponent {
    glm::mat4 transform;
    TransformComponent() = default;
    TransformComponent(const TransformComponent& transform) = default;
    TransformComponent(glm::mat4 transform) : transform(transform) {}
    operator glm::mat4() { return transform; }
    operator const glm::mat4() const { return transform; }
};
} // namespace Chimera
#endif