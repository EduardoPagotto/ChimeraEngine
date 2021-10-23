#include "chimera/render/scene/CameraController.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void CameraController::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera;
    camera->updateDistanceFront();
    camera->updateVectors();
}

void CameraController::onDestroy() {}

void CameraController::onUpdate(float deltaTime) { camera->processInput(deltaTime); }
} // namespace Chimera