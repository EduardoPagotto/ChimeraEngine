#ifndef __CHIMERA_CAMERA_CONTROLLER_FPS_ZZ1__HPP
#define __CHIMERA_CAMERA_CONTROLLER_FPS_ZZ1__HPP

#include "Components.hpp"
#include "ScriptableEntity.hpp"

#include <iostream>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

class CameraControllerFPS : public ScriptableEntity {
  public:
    CameraControllerFPS() {}
    void onCreate();
    void onDestroy();
    void onUpdate(float time);

    void setParams(const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
                   const float& pitch = 0.0f, const float& yaw = -90.0f, const float& fov = 45.0f) {
        this->front = front;
        this->up = up;
        this->pitch = pitch;
        this->yaw = yaw;
        this->fov = fov;
    }

  private:
    void updateVectors();
    void recalculateMatrix(bool left);
    void invertPitch();
    void processCameraFOV(double offset);
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    void processCameraMovement(glm::vec3& direction, float deltaTime);

    Camera* camera;
    glm::vec3 front, up, right, worldUp;
    float pitch, yaw, fov;
    float movementSpeed;
};

} // namespace Chimera

#endif