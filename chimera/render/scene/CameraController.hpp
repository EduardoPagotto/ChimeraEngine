#ifndef __CHIMERA_CAMERA_CONTROLLER_ZZ1__HPP
#define __CHIMERA_CAMERA_CONTROLLER_ZZ1__HPP

#include "Components.hpp"
#include "ScriptableEntity.hpp"

#include <iostream>

namespace Chimera {

class CameraController : public ScriptableEntity {
  public:
    void onCreate() { std::cout << "On CREATE SOCORRO!!!!!!!" << std::endl; }

    void onDestroy() {}
    void onUpdate(float time) {

        auto& transform = getComponent<TransformComponent>().transform;

        // modificar para FPSCamera e orbit camera!!!!!

        std::cout << "On update SOCORRO!!!!!!!" << std::endl;
    }

  private:
};

} // namespace Chimera

#endif