#pragma once
#include "ScriptableEntity.hpp"
#include "chimera/core/visible/ICamera.hpp"
#include "chimera/core/visible/ITrans.hpp"
#include "chimera/core/visible/Light.hpp"
#include "chimera/core/visible/Material.hpp"
#include "chimera/core/visible/Mesh.hpp"
#include "chimera/render/3d/IRenderable3d.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Chimera {

struct SpriteComponent {
    glm::vec4 color;
    SpriteComponent(const glm::vec4& color) : color(color) {} // 15:07
    SpriteComponent(const SpriteComponent& other) = default;
};

struct CameraComponent {
    ICamera* camera = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    bool single = true; // FIXME: Camera simples ou dupla aqui, melhorar para o arquivo .DAE
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    // CameraComponent(glm::mat4 projection) : camera(projection) {}
};

struct TransComponent {
    ITrans* trans = nullptr;
    bool solid = false;
    TransComponent() = default;
};

struct LightComponent {
    Light* light = nullptr;
    bool global = true;
    LightComponent() = default;
};

struct MeshComponent {
    TagComponent tag;
    Mesh* mesh = nullptr;
    MeshComponent() = default;
};

struct MaterialComponent {
    TagComponent tag;
    Material* material = nullptr;
    MaterialComponent() = default;
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
