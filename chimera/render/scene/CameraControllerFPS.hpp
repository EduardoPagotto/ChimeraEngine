#ifndef __CHIMERA_CAMERA_CONTROLLER_FPS_ZZ1__HPP
#define __CHIMERA_CAMERA_CONTROLLER_FPS_ZZ1__HPP

#include "ScriptableEntity.hpp"
#include "chimera/core/CameraFPS.hpp"

#include <iostream>

namespace Chimera {

class CameraControllerFPS : public ScriptableEntity {
  public:
    CameraControllerFPS() = default;
    void onCreate();
    void onDestroy();
    void onUpdate(float time);

  private:
    CameraFPS* camera = nullptr;
};

} // namespace Chimera

#endif