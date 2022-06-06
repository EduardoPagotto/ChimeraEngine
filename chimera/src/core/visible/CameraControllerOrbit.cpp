#include "chimera/core/visible/CameraControllerOrbit.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"

namespace Chimera {

void CameraControllerOrbit::onCreate() {

    // if (cc.d.camKind==CamKind::FPS) // FIXME: Fazer antes acima
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera; // FIXME: na criacao de camera se define se Perspective ou OrthogonaleyeView->
    eyeView = cc.eyeView;
    up = cc.up;
    pitch = cc.pitch;
    yaw = cc.yaw;
    min = cc.min;
    max = cc.max;
    front = glm::vec3(0, 0, 0);
    distance = glm::distance(camera->getPosition(), this->front);

    this->updateVectors();
}

void CameraControllerOrbit::onDestroy() {}

void CameraControllerOrbit::onUpdate(const double& ts) { this->update(ts); }

void CameraControllerOrbit::updateEye() {
    if (eyeView->size() == 1) {
        eyeView->update(glm::lookAt(camera->getPosition(), front, up), camera->getProjection());
    } else {
        glm::vec3 novaPositionL, novaFrontL, novaPositionR, novaFrontR;
        glm::vec3 left_p = front - camera->getPosition(); // front and position as points
        glm::vec3 cross1 = glm::cross(up, left_p);
        glm::vec3 norm1 = glm::normalize(cross1);
        glm::vec3 final_norm1 = norm1 * eyeView->getNoseDist();

        novaPositionL = camera->getPosition() + final_norm1;
        novaFrontL = front + final_norm1;
        eyeView->getHead()[0].update(glm::lookAt(novaPositionL, novaFrontL, up), camera->getProjection()); // Left

        novaPositionR = camera->getPosition() - final_norm1;
        novaFrontR = front - final_norm1;
        eyeView->getHead()[1].update(glm::lookAt(novaPositionR, novaFrontR, up), camera->getProjection()); // Right
    }
}

void CameraControllerOrbit::updateVectors() {

    float theta = glm::radians(yaw); // yaw * 0.017453293f; ( yaw * (PI/180) )
    float phi = glm::radians(pitch); // pitch * 0.017453293f;
    glm::vec3 pos;
    if (this->up.y == 1) {
        pos.x = distance * sin(phi) * sin(theta);
        pos.y = distance * cos(phi);
        pos.z = distance * sin(phi) * cos(theta);
    } else { // this->up.z == 1 ou -1
        pos.x = distance * cos(theta) * sin(phi);
        pos.y = distance * cos(theta) * cos(phi);
        pos.z = distance * sin(theta);
    }

    camera->setPosition(pos);
}

void CameraControllerOrbit::processDistance(const int& _mz) {
    distance += _mz;
    if (distance < min)
        distance = min;
    if (distance > max)
        distance = max;
}

void CameraControllerOrbit::processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch) {

    if (this->up.y == 1) {
        yaw -= (float)xOffset;
        pitch -= (float)yOffset;

        // Constrain the pitch
        if (constrainPitch) {
            if (pitch < 1.0f)
                pitch = 1.0f;
            if (pitch > 179.0f)
                pitch = 179.0f;
        }

    } else { // this->->up.z == 1 ou -1

        yaw += (float)yOffset;
        pitch += (float)xOffset;
        // if (yaw < 1.0f)
        //     yaw = 1.0f;
        // if (yaw > 179.0f)
        //     yaw = 179.0f;
    }
}

void CameraControllerOrbit::update(const double& ts) {
    if (MouseDevice::getButtonState(1) == SDL_PRESSED) {
        glm::ivec2 mouseMove = MouseDevice::getMoveRel();
        this->processCameraRotation(mouseMove.x, mouseMove.y);

    } else if (MouseDevice::getButtonState(3) == SDL_PRESSED) {
        glm::ivec2 mouseMove = MouseDevice::getMoveRel();
        this->processDistance(mouseMove.y);
    }

    updateVectors();

    this->updateEye();
}

void CameraControllerOrbit::invertPitch() {
    pitch = -pitch;
    updateVectors();
}

} // namespace Chimera