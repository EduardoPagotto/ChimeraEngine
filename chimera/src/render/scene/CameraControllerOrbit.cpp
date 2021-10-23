#include "chimera/render/scene/CameraControllerOrbit.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void CameraControllerOrbit::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = (CameraOrbit*)cc.camera;
    camera->updateDistanceFront();
    camera->updateVectors();
}

void CameraControllerOrbit::onDestroy() {}

void CameraControllerOrbit::onUpdate(float deltaTime) { camera->processInput(deltaTime); }
} // namespace Chimera