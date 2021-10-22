#ifndef __CHIMERA_COMPONENTS__HPP
#define __CHIMERA_COMPONENTS__HPP

#include "ScriptableEntity.hpp"
#include "chimera/core/Camera.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f

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

struct CameraControlerFPSParams {
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float pitch = 0.0f, yaw = -90.0f, fov = 45.0f;
    CameraControlerFPSParams() = default;
};

struct CameraControlerOrbitParams {
    glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float pitch = 0.0f, yaw = -90.0f, fov = 45.0f;
    float min = 1.0f, max = 500.0f;
    CameraControlerOrbitParams() = default;
};

struct CameraComponent {
    Camera camera;
    bool primary = true;
    bool fixedAspectRatio = false;
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    // CameraComponent(glm::mat4 projection) : camera(projection) {}
};

struct Renderable3dComponent {
    IRenderable3d* renderable;
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