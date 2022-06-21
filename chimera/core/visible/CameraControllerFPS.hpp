#pragma once
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraControllerFPS : public ScriptableEntity {
  public:
    CameraControllerFPS() = default;
    void onCreate() override;
    void onDestroy() override;
    void onUpdate(const double& ts) override;

  private:
    void updateEye();
    void updateVectors();
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    void processCameraMovement(glm::vec3& direction, float deltaTime);
    void invertPitch();

    float pitch, yaw, movementSpeed;
    glm::vec3 up, front, worldUp, right;
    Camera* camera = nullptr;
    EyeView* eyeView = nullptr;
};
} // namespace Chimera