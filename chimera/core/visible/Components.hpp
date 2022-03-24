#pragma once
#include "ICamera.hpp"
#include "ITrans.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

namespace Chimera {

// struct TransComponent {
//     ITrans* trans = nullptr;
//     bool solid = false;
//     TransComponent() = default;
// };

// struct LightComponent {
//     TagComponent tag;
//     Light* light = nullptr;
//     bool global = true;
//     LightComponent() = default;
// };

// struct CameraComponent {
//     TagComponent tag;
//     ICamera* camera = nullptr;
//     bool primary = true;
//     bool fixedAspectRatio = false;
//     bool single = true; // FIXME: Camera simples ou dupla aqui, melhorar para o arquivo .DAE
//     CameraComponent() = default;
//     CameraComponent(const CameraComponent&) = default;
//     // CameraComponent(glm::mat4 projection) : camera(projection) {}
// };

// struct MeshComponent {
//     TagComponent tag;
//     Mesh* mesh = nullptr;
//     MeshComponent() = default;
// };

// struct MaterialComponent {
//     TagComponent tag;
//     Material* material = nullptr;
//     MaterialComponent() = default;
// };

struct CameraComponent {
    TagComponent tag;
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
    TagComponent tag;
    Light* light = nullptr;
    bool global = true;
    LightComponent() = default;
};

struct MeshComponent {
    TagComponent tag;
    Mesh* mesh = new Mesh();
    MeshComponent() = default;
};

struct MaterialComponent {
    TagComponent tag;
    Material* material = new Material();
    MaterialComponent() = default;
};

} // namespace Chimera