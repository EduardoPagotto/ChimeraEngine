#ifndef __CHIMERA_COMPONENTS__HPP
#define __CHIMERA_COMPONENTS__HPP

#include "ScriptableEntity.hpp"
#include "chimera/core/Camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

// struct TransformComponent {
//     glm::mat4 transform;
//     TransformComponent() = default;
//     TransformComponent(const TransformComponent& transform) = default;
//     TransformComponent(glm::mat4 transform) : transform(transform) {}
//     operator glm::mat4() { return transform; }
//     operator const glm::mat4() const { return transform; }
// };

struct TransformComponent {

    glm::vec3 translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent& transform) = default;
    // TransformComponent(glm::mat4 transform) : transform(transform) {}
    // operator glm::mat4() { return transform; }

    glm::mat4 getTransform() const {

        glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation.x, {1, 0, 0}) * glm::rotate(glm::mat4(1.0f), rotation.y, {0, 1, 0}) *
                        glm::rotate(glm::mat4(1.0f), rotation.z, {0, 0, 1});

        return glm::translate(glm::mat4(1.0f), translation) * rot * glm::scale(glm::mat4(1.0f), scale);
    }
};

struct CameraComponent {
    Camera camera;
    bool primary = true;
    bool fixedAspectRatio = false;
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    // CameraComponent(glm::mat4 projection) : camera(projection) {}
};

struct NativeScriptComponent {

    std::string name;
    ScriptableEntity* instance = nullptr;

    ScriptableEntity* (*instantiateScript)();
    void (*destroyScript)(NativeScriptComponent*);

    template <typename T> void bind(const std::string& nameBind) {

        name = nameBind;

        instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };

        destroyScript = [](NativeScriptComponent* nsc) {
            delete nsc->instance;
            nsc->instance = nullptr;
        };
    }
};

} // namespace Chimera
#endif