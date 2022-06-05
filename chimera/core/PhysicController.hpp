#pragma once
#include "chimera/core/ScriptableEntity.hpp"
#include "chimera/core/bullet/PhysicsControl.hpp"

namespace Chimera {

class PhysicController : public ScriptableEntity {
  public:
    PhysicController() = default;
    void onCreate() override;
    void onDestroy() override;
    void onUpdate(const double& ts) override;

  private:
    PhysicsControl* physicsControl = nullptr;
};
} // namespace Chimera
