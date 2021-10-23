#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/core/io/Keyboard.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void CameraControllerFPS::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = (CameraFPS*)cc.camera;
    camera->updateVectors();
}

void CameraControllerFPS::onDestroy() {}

void CameraControllerFPS::onUpdate(float deltaTime) { camera->processInput(deltaTime); }
} // namespace Chimera