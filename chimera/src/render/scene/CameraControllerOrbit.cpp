#include "chimera/render/scene/CameraControllerOrbit.hpp"
#include "chimera/core/io/Keyboard.hpp"
#include "chimera/core/io/MouseDevice.hpp"
#include "chimera/render/scene/Components.hpp"

namespace Chimera {

void CameraControllerOrbit::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = (CameraOrbit*)cc.camera;
    camera->updateDistanceFront();
    camera->updateVectors();
}

void CameraControllerOrbit::onDestroy() {}

// void CameraControllerOrbit::recalculateMatrix(bool left) { // windows x->width; y -> height

//     glm::mat4 viewMatrix;

//     if (left == false) {
//         viewMatrix = glm::lookAt(camera->getPosition(), cp->front, cp->up);

//     } else {
//         glm::vec3 novaPosition = camera->getPosition();
//         glm::vec3 novaFront = cp->front;
//         glm::vec3 left_p = novaFront - novaPosition;
//         glm::vec3 cross1 = glm::cross(cp->up, left_p);
//         glm::vec3 norm1 = glm::normalize(cross1);
//         glm::vec3 final_norm1 = norm1 * 5.0f;

//         novaPosition = camera->getPosition() - final_norm1;
//         novaFront = cp->front - final_norm1;

//         viewMatrix = glm::lookAt(novaPosition, novaFront, cp->up);
//     }

//     glm::mat4 projectionMatrix = glm::perspective(glm::radians(cp->fov), camera->getAspectRatio(), camera->getNear(), camera->getFar());

//     camera->updateAllMatrix(projectionMatrix, viewMatrix);
// }

// void CameraControllerOrbit::invertPitch() {
//     cp->pitch = -cp->pitch;
//     updateVectors();
// }

// void CameraControllerOrbit::updateVectors() {

//     glm::vec3 pos;
//     float theta = glm::radians(cp->yaw); // yaw * 0.017453293f; ( yaw * (PI/180) )
//     float phi = glm::radians(cp->pitch); // pitch * 0.017453293f;
//     if (cp->up.y == 1) {
//         pos.x = distance * sin(phi) * sin(theta);
//         pos.y = distance * cos(phi);
//         pos.z = distance * sin(phi) * cos(theta);
//     } else { // this->up.z == 1 ou -1
//         pos.x = distance * cos(theta) * sin(phi);
//         pos.y = distance * cos(theta) * cos(phi);
//         pos.z = distance * sin(theta);
//     }

//     camera->setPosition(pos);
// }

void CameraControllerOrbit::onUpdate(float deltaTime) {

    camera->processInput(deltaTime);
    // if (MouseDevice::getButtonState(1) == SDL_PRESSED) {
    //     glm::ivec2 mouseMove = MouseDevice::getMoveRel();
    //     processCameraRotation(mouseMove.x, mouseMove.y);

    // } else if (MouseDevice::getButtonState(3) == SDL_PRESSED) {
    //     glm::ivec2 mouseMove = MouseDevice::getMoveRel();
    //     processDistance(mouseMove.y);
    // }

    // this->updateVectors();
    // this->recalculateMatrix(false);
}

// void CameraControllerOrbit::processDistance(const int& _mz) {
//     distance += _mz;
//     if (distance < cp->min)
//         distance = cp->min;
//     if (distance > cp->max)
//         distance = cp->max;
// }

// void CameraControllerOrbit::processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch) {
//     // Make sure the user isn't interacting with the UI
//     // if (!Window::GetHideCursor())
//     //    return;
//     if (cp->up.y == 1) {
//         cp->yaw -= (float)xOffset;
//         cp->pitch -= (float)yOffset;

//         // Constrain the pitch
//         if (constrainPitch) {
//             if (cp->pitch < 1.0f)
//                 cp->pitch = 1.0f;
//             if (cp->pitch > 179.0f)
//                 cp->pitch = 179.0f;
//         }

//     } else { // this->->up.z == 1 ou -1

//         cp->yaw += (float)yOffset;
//         cp->pitch += (float)xOffset;
//         // if (yaw < 1.0f)
//         //     yaw = 1.0f;
//         // if (yaw > 179.0f)
//         //     yaw = 179.0f;
//     }
// }

// void CameraControllerOrbit::processCameraFOV(double offset) {

//     if (offset != 0.0 && cp->fov >= 1.0 && cp->fov <= CAMERA_MAX_FOV) {
//         cp->fov -= (float)offset;
//     }
//     if (cp->fov < 1.0f) {
//         cp->fov = 1.0f;
//     } else if (cp->fov > CAMERA_MAX_FOV) {
//         cp->fov = CAMERA_MAX_FOV;
//     }
// }

} // namespace Chimera