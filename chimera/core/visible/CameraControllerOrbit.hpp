#pragma once
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraControllerOrbit : public ScriptableEntity {
  public:
    CameraControllerOrbit() = default;
    void onCreate() override;
    void onDestroy() override;
    void onUpdate(const double& ts) override;

  private:
    void updateEye();
    void updateVectors();
    void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
    void processDistance(const int& _mz);
    void invertPitch();

    float pitch, yaw, distance, min, max;
    glm::vec3 up, front;
    ICamera* camera = nullptr;
    EyeView* eyeView = nullptr;
};

} // namespace Chimera
