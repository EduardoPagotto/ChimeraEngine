#pragma once
#include "ICamera.hpp"

namespace Chimera {

class CameraFPS : public ICamera3D {

  public:
    CameraFPS(const glm::vec3& pos, const glm::vec3& up, float yaw = -90.0f, float pitch = 0.0f);
    virtual ~CameraFPS();

    void processCameraFOV(double yOffset);
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch = true);
    void processCameraMovement(glm::vec3& direction, float deltaTime);
    virtual void onUpdate(const double& ts) override;
    virtual const glm::mat4& getProjection() const override { return projectionMatrix; }
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::vec3& getFront() const override { return front; }
    virtual const glm::vec3& getUp() const override { return up; }
    virtual void setFov(const float& value) override { this->fov = value; }
    virtual void setNear(const float& value) override { this->nearPlane = value; }
    virtual void setFar(const float& value) override { this->farPlane = value; }
    virtual float updateDistanceFront() override { return glm::distance(this->position, this->front); }
    virtual void setPosition(const glm::vec3& position) override { this->position = position; }
    virtual void invertPitch() override;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override;
    virtual void updateVectors() override;
    virtual const bool is3D() const override { return true; }
    virtual EyeView* getEyeView() override { return &eye; }

  private:
    void updateEye();
    glm::vec3 position, front, up, right, worldUp;
    float pitch, yaw, fov;
    float nearPlane, farPlane;
    float movementSpeed;
    glm::mat4 projectionMatrix;
    EyeView eye;
};
} // namespace Chimera
