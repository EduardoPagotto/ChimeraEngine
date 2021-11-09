#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

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

void CameraController::onUpdate(const uint32_t& count) { camera->onUpdate(count); }
} // namespace Chimera