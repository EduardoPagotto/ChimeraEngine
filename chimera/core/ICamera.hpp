#ifndef __CHIMERA_ICAMERA_ZZ_HPP
#define __CHIMERA_ICAMERA_ZZ_HPP

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
    virtual const glm::vec3& getFront() const = 0;
    virtual const glm::vec3& getUp() const = 0;
    virtual float updateDistanceFront() = 0;
    virtual void setPosition(const glm::vec3& position) = 0;
    virtual void invertPitch() = 0;
    virtual void setFov(const float& value) = 0;
    virtual void setNear(const float& value) = 0;
    virtual void setFar(const float& value) = 0;
    virtual void setAspectRatio(const uint32_t& width, const uint32_t& height) = 0;
    virtual void processInput(float deltaTime) = 0; // FIXME: achar outra saida!!
    virtual void updateVectors() = 0;
};
} // namespace Chimera
#endif