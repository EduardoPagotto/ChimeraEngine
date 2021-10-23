#ifndef __CHIMERA_CAMERA_ORBIT_HPP
#define __CHIMERA_CAMERA_ORBIT_HPP

#include "ICamera.hpp"

namespace Chimera {

class CameraOrbit : public ICamera {

  public:
    CameraOrbit(const glm::vec3& pos, const glm::vec3& up, float yaw = -90.0f, float pitch = 0.0f);
    virtual ~CameraOrbit();

    void processCameraFOV(double yOffset);
    void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
    void processDistance(const int& _mz);
    void setLimits(const float& min, const float& max) {
        this->min = min;
        this->max = max;
    }

    // herdado
    virtual const glm::mat4 getViewMatrix() const override { return viewMatrix; };
    virtual const glm::mat4 getProjectionMatrix() const override { return projectionMatrix; }
    virtual const glm::mat4 getViewProjectionMatrix() const override { return viewProjectionMatrix; };
    virtual const glm::mat4 getViewProjectionMatrixInverse() const override { return viewProjectionMatrixInverse; };
    virtual const glm::mat4 recalculateMatrix(bool left) override;
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::vec3& getFront() const override { return front; }
    virtual const glm::vec3& getUp() const override { return up; }
    virtual void setFov(const float& value) override { this->fov = value; }
    virtual void setNear(const float& value) override { this->nearPlane = value; }
    virtual void setFar(const float& value) override { this->farPlane = value; }
    virtual void processInput(float deltaTime) override;
    virtual void setPosition(const glm::vec3& position) override { this->position = position; }
    virtual void invertPitch() override;
    virtual void setAspectRatio(const uint32_t& width, const uint32_t& height) override { aspectRatio = (float)width / (float)height; }
    virtual void updateVectors() override;
    virtual float updateDistanceFront() override {
        distance = glm::distance(this->position, this->front);
        return distance;
    }

  private:
    glm::vec3 position, front, up;
    float pitch, yaw, fov;
    float nearPlane, farPlane;
    float distance;
    float min, max, aspectRatio;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 viewProjectionMatrixInverse;
};
} // namespace Chimera
#endif