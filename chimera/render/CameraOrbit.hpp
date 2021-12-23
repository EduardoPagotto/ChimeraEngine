#pragma once
#include "ICamera.hpp"

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

    virtual const glm::mat4& getViewMatrix() const override { return eye.getView(); }
    virtual const glm::mat4& getProjectionMatrix() const override { return projectionMatrix; }
    virtual const glm::mat4& getViewProjectionMatrix() const override { return eye.getViewProjection(); }
    virtual const glm::mat4& getViewProjectionMatrixInverse() const override { return eye.getViewProjectionInverse(); }
    virtual const glm::mat4& recalculateMatrix(const uint8_t& eyeIndex) override;
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
    virtual const bool is3D() const { return true; };

  private:
    glm::vec3 position, front, up;
    float pitch, yaw, fov;
    float nearPlane, farPlane;
    float distance;
    float min, max;
    glm::mat4 projectionMatrix;
    EyeView eye;
};
} // namespace Chimera
