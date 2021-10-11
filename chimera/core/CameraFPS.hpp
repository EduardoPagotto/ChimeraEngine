#ifndef __CHIMERA_CAMERA_FPS_HPP
#define __CHIMERA_CAMERA_FPS_HPP

#include "ICamera.hpp"

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f

class CameraFPS : public ICamera {

  public:
    CameraFPS(const glm::vec3& pos, const glm::vec3& up, float yaw = -90.0f, float pitch = 0.0f);
    virtual ~CameraFPS();

    void processCameraFOV(double yOffset);
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch = true);
    void processCameraMovement(glm::vec3& direction, float deltaTime);

    virtual void processInput(float deltaTime) override;

    // herdado
    virtual glm::mat4 getViewMatrix() override;
    glm::mat4 getProjectionMatrix(const glm::ivec2& res) override;
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
};
} // namespace Chimera
#endif