#pragma once
#include "ScriptableEntity.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

struct SpriteComponent {
    glm::vec4 color;
    SpriteComponent(const glm::vec4& color) : color(color) {} // 15:07
    SpriteComponent(const SpriteComponent& other) = default;
};

struct Renderable3dComponent {
    IRenderable3d* renderable;
};

struct RenderableParticlesComponent {
    IRenderable3d* renderable;
    bool enable;
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
