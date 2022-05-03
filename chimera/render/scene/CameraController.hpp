#pragma once
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraController : public ScriptableEntity {
  public:
    CameraController() = default;
    void onCreate() override;
    void onDestroy() override;
    void onUpdate(const double& ts) override;

  private:
    ICamera* camera = nullptr;
};
} // namespace Chimera
