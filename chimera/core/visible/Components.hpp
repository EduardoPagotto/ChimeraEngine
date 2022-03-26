#pragma once
#include "ICamera.hpp"
#include "ITrans.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

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

} // namespace Chimera