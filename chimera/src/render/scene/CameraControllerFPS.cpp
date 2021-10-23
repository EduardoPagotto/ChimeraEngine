#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void CameraControllerFPS::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera;
    camera->updateDistanceFront();
    camera->updateVectors();
}

void CameraControllerFPS::onDestroy() {}

void CameraControllerFPS::onUpdate(float deltaTime) { camera->processInput(deltaTime); }
} // namespace Chimera