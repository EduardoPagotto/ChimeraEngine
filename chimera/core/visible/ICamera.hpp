#pragma once
#include "chimera/core/TagComponent.hpp"
#include "chimera/core/visible/EyeView.hpp"

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

class ICamera {
  public:
    virtual ~ICamera() {}
    virtual const glm::mat4& getProjection() const = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void update(const double& ts, EyeView* eyeView) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
    virtual const bool is3D() const = 0;
};

struct CameraComponent {
    TagComponent tag;
    ICamera* camera = nullptr;
    EyeView* eyeView = nullptr;
    bool primary = true;
    bool fixedAspectRatio = false;
    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
};

class ICamera3D : public ICamera {
  public:
    virtual ~ICamera3D() {}
    virtual const glm::vec3& getFront() const = 0;
    virtual const glm::vec3& getUp() const = 0;
    virtual float updateDistanceFront() = 0;
    virtual void invertPitch() = 0;
    virtual void setFov(const float& value) = 0;
    virtual void setNear(const float& value) = 0;
    virtual void setFar(const float& value) = 0;
    virtual void updateVectors() = 0;
};
} // namespace Chimera
