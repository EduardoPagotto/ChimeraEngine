#ifndef __CHIMERA_CAMERA_CONTROLLER_ORBIT_ZZ1__HPP
#define __CHIMERA_CAMERA_CONTROLLER_ORBIT_ZZ1__HPP

#include "Components.hpp"
#include "ScriptableEntity.hpp"

#include <iostream>

namespace Chimera {

#define FPSCAMERA_MAX_SPEED 40.0f
#define FPSCAMERA_ROTATION_SENSITIVITY 0.3f
#define CAMERA_MAX_FOV 45.0f

class CameraControllerOrbit : public ScriptableEntity {
  public:
    CameraControllerOrbit() = default;
    void onCreate();
    void onDestroy();
    void onUpdate(float time);

  private:
    void updateVectors();
    void recalculateMatrix(bool left);
    void invertPitch();

    void processDistance(const int& _mz);
    void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
    void processCameraFOV(double offset);

    Camera* camera = nullptr;
    CameraControlerOrbitParams* cp = nullptr;
    float distance = 0.0f;
};

} // namespace Chimera

#endif