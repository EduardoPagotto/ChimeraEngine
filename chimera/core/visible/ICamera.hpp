#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/visible/EyeView.hpp"
#include "chimera/core/visible/Projection.hpp"

namespace Chimera {

class ICamera {
  public:
    virtual const glm::mat4& getProjection() const = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
};

class Camera : public ICamera {
  public:
    Camera(Projection* p) : projection(p) {}
    virtual const glm::mat4& getProjection() const override { return projection->getProjection(); };
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual void setPosition(const glm::vec3& position) override { this->position = position; }
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override {
        projection->setViewportProjection(width, height);
    }

  private:
    Projection* projection;
    glm::vec3 position;
};

struct CameraComponent {
    TagComponent tag;
    ICamera* camera = nullptr;
    EyeView* eyeView = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    CamKind camKind = CamKind::STATIC;
    float pitch = 0.0f, yaw = 90.0f, min = 0.5f, max = 1000.0f;
    glm::vec3 up = glm::vec3(0, 1, 0);
    CameraComponent() = default;
    CameraComponent(const CameraComponent& o) = default;
};
} // namespace Chimera
