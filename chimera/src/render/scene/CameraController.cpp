#include "chimera/render/scene/CameraController.hpp"

namespace Chimera {

void CameraController::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera;

    if (camera->is3D()) {
        ICamera3D* pc = (ICamera3D*)camera;
        pc->updateDistanceFront();
        pc->updateVectors();
    }
}

void CameraController::onDestroy() {}

void CameraController::onUpdate(const double& ts) { camera->onUpdate(ts); }
} // namespace Chimera