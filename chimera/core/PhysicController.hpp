#pragma once
#include "PhysicsControl.hpp"
#include "ScriptableEntity.hpp"

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
