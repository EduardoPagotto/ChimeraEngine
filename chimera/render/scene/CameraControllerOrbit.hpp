#ifndef __CHIMERA_CAMERA_CONTROLLER_ORBIT_ZZ1__HPP
#define __CHIMERA_CAMERA_CONTROLLER_ORBIT_ZZ1__HPP

#include "ScriptableEntity.hpp"
#include "chimera/core/CameraOrbit.hpp"
#include <iostream>

namespace Chimera {

class CameraControllerOrbit : public ScriptableEntity {
  public:
    CameraControllerOrbit() = default;
    void onCreate();
    void onDestroy();
    void onUpdate(float time);

  private:
    CameraOrbit* camera = nullptr;
};

} // namespace Chimera

#endif