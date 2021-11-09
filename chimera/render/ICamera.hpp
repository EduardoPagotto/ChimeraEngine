#pragma once
#include <glm/glm.hpp>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

class ICamera {
  public:
    virtual ~ICamera() {}
    virtual const glm::mat4 getViewMatrix() const = 0;
    virtual const glm::mat4 getProjectionMatrix() const = 0;
    virtual const glm::mat4 getViewProjectionMatrix() const = 0;
    virtual const glm::mat4 getViewProjectionMatrixInverse() const = 0;
    virtual const glm::mat4 recalculateMatrix(bool left) = 0;
    virtual const glm::vec3& getPosition() const = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void onUpdate(const double& ts) = 0;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) = 0;
    virtual const bool is3D() const = 0;
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
