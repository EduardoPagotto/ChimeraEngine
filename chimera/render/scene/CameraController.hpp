#pragma once

#include "ScriptableEntity.hpp"
#include "chimera/render/ICamera.hpp"

namespace Chimera {

class CameraController : public ScriptableEntity {
  public:
    CameraController() = default;
    void onCreate();
    void onDestroy();
    void onUpdate(float time);

  private:
    ICamera* camera = nullptr;
};
} // namespace Chimera
