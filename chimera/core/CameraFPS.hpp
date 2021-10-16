#ifndef __CHIMERA_CAMERA_FPS_HPP
#define __CHIMERA_CAMERA_FPS_HPP

#include "ICamera.hpp"

namespace Chimera {

class CameraFPS : public ICamera {

  public:
    CameraFPS(const glm::vec3& pos, const glm::vec3& up, float yaw = -90.0f, float pitch = 0.0f);
    virtual ~CameraFPS();

    void processCameraFOV(double yOffset);
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch = true);
    void processCameraMovement(glm::vec3& direction, float deltaTime);

    virtual void processInput(float deltaTime) override;

    // herdado
    virtual const glm::mat4 getViewMatrix() const override { return viewMatrix; };
    virtual const glm::mat4 getProjectionMatrix() const override { return projectionMatrix; }
    virtual const glm::mat4 getViewProjectionMatrix() const { return viewProjectionMatrix; };
    virtual const glm::mat4 getViewProjectionMatrixInverse() const { return viewProjectionMatrixInverse; };
    virtual glm::mat4 recalculateMatrix(const float& canvasRatio = 1.0f, bool left = false) override;
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::vec3& getFront() const override { return front; }
    virtual const glm::vec3& getUp() const override { return up; }
    virtual void setParams(const float& fov, const float& near, const float& far) override;
    virtual float updateDistanceFront() override { return glm::distance(this->position, this->front); }
    virtual void setPosition(const glm::vec3& position) override { this->position = position; }
    virtual void invertPitch() override;

  private:
    void updateVectors();

    glm::vec3 position, front, up, right, worldUp;
    float pitch, yaw, fov;
    float nearPlane, farPlane;
    float movementSpeed;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 viewProjectionMatrix;
    glm::mat4 viewProjectionMatrixInverse;
};
} // namespace Chimera
#endif