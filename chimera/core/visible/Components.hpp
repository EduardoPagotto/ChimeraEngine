#pragma once
#include "ICamera.hpp"
#include "ITrans.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "chimera/core/ScriptableEntity.hpp"

namespace Chimera {

struct ComponentCamera {
    TagComponent tag;
    ICamera* camera = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    bool single = true; // FIXME: Camera simples ou dupla aqui, melhorar para o arquivo .DAE
    ComponentCamera() = default;
    ComponentCamera(const ComponentCamera&) = default;
    // ComponentCamera(glm::mat4 projection) : camera(projection) {}
};

struct ComponentTrans {
    ITrans* trans = nullptr;
    bool solid = false;
    ComponentTrans() = default;
};

struct ComponentLight {
    TagComponent tag;
    Light* light = new Light();
    bool global = true;
    ComponentLight() = default;
};

struct ComponentMesh {
    TagComponent tag;
    Mesh* mesh = new Mesh();
    ComponentMesh() = default;
};

struct ComponentMaterial {
    TagComponent tag;
    Material* material = new Material();
    ComponentMaterial() = default;
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