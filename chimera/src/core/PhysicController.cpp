#include "chimera/core/PhysicController.hpp"

namespace Chimera {

void PhysicController::onCreate() {
    auto& cc = getComponent<PhysicsControl>();
    physicsControl = &cc;
}

void PhysicController::onDestroy() {}

void PhysicController::onUpdate(const double& ts) {

    physicsControl->stepSim(ts);
    physicsControl->checkCollisions();
}
} // namespace Chimera