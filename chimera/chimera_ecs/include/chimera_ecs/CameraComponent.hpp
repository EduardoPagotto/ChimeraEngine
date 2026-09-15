#pragma once
#include "chimera_base/aux/ICamera.hpp"
#include "ecs.hpp"
#include <memory>

namespace ce {

    struct CameraComponent {
        TagInfo tag;
        std::shared_ptr<Camera> camera;
        bool primary{true};
        bool fixedAspectRatio{false};
        CamKind camKind{CamKind::STATIC};
        float pitch{0.0F};
        float yaw{90.0F};
        float min{0.5F};
        float max{1000.0F};

        glm::vec3 up{glm::vec3(0, 1, 0)};

        CameraComponent() = default;
        CameraComponent(const CameraComponent& o) = default;
    };

} // namespace ce
