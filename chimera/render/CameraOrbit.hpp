#pragma once
#include "chimera/core/ICamera.hpp"

namespace Chimera {

class CameraOrbit : public ICamera3D {

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

    virtual const glm::mat4& getProjection() const override { return projectionMatrix; }
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::vec3& getFront() const override { return front; }
    virtual const glm::vec3& getUp() const override { return up; }
    virtual void setFov(const float& value) override { this->fov = value; }
    virtual void setNear(const float& value) override { this->nearPlane = value; }
    virtual void setFar(const float& value) override { this->farPlane = value; }
    virtual void onUpdate(const double& ts) override;
    virtual void setPosition(const glm::vec3& position) override { this->position = position; }
    virtual void invertPitch() override;
    virtual void setViewportSize(const uint32_t& width, const uint32_t& height) override;
    virtual void updateVectors() override;
    virtual float updateDistanceFront() override {
        distance = glm::distance(this->position, this->front);
        return distance;
    }
    virtual const bool is3D() const override { return true; };
    virtual EyeView* view() override { return &eye; }

  private:
    void updateEye();
    glm::vec3 position, front, up;
    float pitch, yaw, fov;
    float nearPlane, farPlane;
    float distance;
    float min, max;
    glm::mat4 projectionMatrix;
    EyeView eye;
};
} // namespace Chimera
