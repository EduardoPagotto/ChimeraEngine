#pragma once

#include "ScriptableEntity.hpp"
#include "chimera/render/ICamera.hpp"

namespace Chimera {

class CameraController : public ScriptableEntity {
  public:
    CameraController() = default;
    void onCreate();
    void onDestroy();
    void onUpdate(const uint32_t& count) override;

  private:
    ICamera* camera = nullptr;
};
} // namespace Chimera
