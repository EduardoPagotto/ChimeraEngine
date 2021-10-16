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
    CameraControllerFPS() = default;
    void onCreate();
    void onDestroy();
    void onUpdate(float time);

    void recalculateMatrix(bool left);
    void invertPitch();
    void processCameraFOV(double offset);

  private:
    void updateVectors();

    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    void processCameraMovement(glm::vec3& direction, float deltaTime);

    Camera* camera = nullptr;
    CameraControlerFPSParams* cp = nullptr;
    glm::vec3 right = glm::vec3(0.0f), worldUp = glm::vec3(0.0f);
    float movementSpeed = FPSCAMERA_MAX_SPEED;
};

} // namespace Chimera

#endif