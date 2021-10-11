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
    // void processCameraMovement(glm::vec3& direction, float deltaTime);
    void processDistance(const int& _mz);

    void setLimits(const float& min, const float& max) {
        this->min = min;
        this->max = max;
    }

    float updateDistanceFront() override {
        distance = glm::distance(this->position, this->front);
        return distance;
    }

    // herdado
    virtual glm::mat4 getViewMatrix() const override { return viewMatrix; };
    virtual glm::mat4 getProjectionMatrix() const override { return projectionMatrix; }
    virtual glm::mat4 recalcMatrix(const float& canvasRatio) override;
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::vec3& getFront() const override { return front; }
    virtual const glm::vec3& getUp() const override { return up; }
    virtual void setParams(const float& fov, const float& near, const float& far) override;
    virtual void processInput(float deltaTime) override;

    virtual void setPosition(const glm::vec3& position) { this->position = position; }
    virtual void invertPitch();

  private:
    void updateVectors();

    glm::vec3 position, front, up;
    float pitch, yaw, fov;
    float nearPlane, farPlane;
    float distance;
    float min, max;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 viewProjectionMatrixInverse;
};
} // namespace Chimera
#endif