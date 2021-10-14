#ifndef __CHIMERA_COMPONENTS__HPP
#define __CHIMERA_COMPONENTS__HPP

#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"
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

struct CameraComponent {
    SceneCamera camera;
    bool primary = true;
    bool fixedAspectRatio = false;
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    // CameraComponent(glm::mat4 projection) : camera(projection) {}
};

struct NativeScriptComponent {

    ScriptableEntity* instance = nullptr;

    std::function<void()> instantiateFunction;
    std::function<void()> destroyInstanceFunction;

    std::function<void(ScriptableEntity*)> onCreateFunction;
    std::function<void(ScriptableEntity*)> onDestroyFuncion;
    std::function<void(ScriptableEntity*, float)> onUpdateFuncion;

    template <typename T> void bind() {

        instantiateFunction = [&]() { instance = new T(); };

        destroyInstanceFunction = [&]() {
            delete (T*)instance;
            instance = nullptr;
        };

        onCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->onCreate(); };
        onDestroyFuncion = [](ScriptableEntity* instance) { ((T*)instance)->onDestroy(); };
        onUpdateFuncion = [](ScriptableEntity* instance, float ts) { ((T*)instance)->onUpdate(ts); };
    }
};

} // namespace Chimera
#endif