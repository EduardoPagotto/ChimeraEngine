#ifndef __CHIMERA_CAMERA_FINAL_HPP
#define __CHIMERA_CAMERA_FINAL_HPP

#include "ICamera.hpp"

namespace Chimera {

#define FPSCAMERA_MAX_FOV 90.0f
#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.1f

class Camera : public ICamera {

  public:
    // Camera();
    Camera(const glm::vec3& pos, const glm::vec3& up, float yaw = -90.0f, float pitch = 0.0f);
    virtual ~Camera();

    void processCameraFOV(double yOffset);
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch = true);
    void processCameraMovement(glm::vec3& direction, float deltaTime);
    void processInput(float deltaTime);

    // herdado
    virtual glm::mat4 getViewMatrix() override;
    glm::mat4 getProjectionMatrix(const glm::ivec2& res) override;
    virtual const glm::vec3& getPosition() const override { return position; }
    virtual const glm::vec3& getFront() const override { return front; }
    virtual const glm::vec3& getUp() const override { return up; }

    virtual void setPosition(const glm::vec3& position) { this->position = position; }
    virtual void invertPitch();

  private:
    void updateVectors();

    // glm::vec3 m_Position, m_Front, m_Up, m_Right, m_WorldUp;
    glm::vec3 position, front, up, right, worldUp;
    float pitch, yaw, fov;
    // Frustum Options
    float nearPlane, farPlane;

    float movementSpeed; // m_CurrentMovementSpeed;
};
} // namespace Chimera
#endif