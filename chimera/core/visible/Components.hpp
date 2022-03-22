#pragma once
#include "ICamera.hpp"
#include "ITrans.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

namespace Chimera {

struct ComponentTrans {
    ITrans* trans = nullptr;
    bool solid = false;
    ComponentTrans() = default;
};

struct ComponentLight {
    TagComponent tag;
    Light* light = nullptr;
    bool global = true;
    ComponentLight() = default;
};

struct ComponentCamera {
    TagComponent tag;
    ICamera* camera = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    bool single = true; // FIXME: Camera simples ou dupla aqui, melhorar para o arquivo .DAE
    ComponentCamera() = default;
    ComponentCamera(const ComponentCamera&) = default;
    // CameraComponent(glm::mat4 projection) : camera(projection) {}
};

struct ComponentMesh {
    TagComponent tag;
    Mesh* mesh = nullptr;
    ComponentMesh() = default;
};

struct ComponentMaterial {
    TagComponent tag;
    Material* material = nullptr;
    ComponentMaterial() = default;
};

} // namespace Chimera