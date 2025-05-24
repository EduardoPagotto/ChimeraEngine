#pragma once
#include "chimera/base/ICamera.hpp"

namespace ce {

    struct CameraComponent {
        TagInfo tag;
        Camera* camera = nullptr;
        bool primary = true;
        bool fixedAspectRatio = false;
        CamKind camKind = CamKind::STATIC;
        float pitch = 0.0f, yaw = 90.0f, min = 0.5f, max = 1000.0f;
        glm::vec3 up = glm::vec3(0, 1, 0);
        CameraComponent() = default;
        CameraComponent(const CameraComponent& o) = default;
    };

} // namespace ce
