#include "chimera/render/scene/CameraController.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"

namespace Chimera {

void CameraController::onCreate() {
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera;
    eyeView = cc.eyeView;

    if (camera->is3D()) {
        ICamera3D* pc = (ICamera3D*)camera;
        pc->updateDistanceFront();
        pc->updateVectors();
    }
}

void CameraController::onDestroy() {}

void CameraController::onUpdate(const double& ts) { camera->update(ts, eyeView); }

//--

void CameraControllerFPS::onCreate() {

    // if (cc.d.camKind==CamKind::FPS) // FIXME: Fazer antes acima
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera; // FIXME: na criacao de camera se define se Perspective ou OrthogonaleyeView->
    eyeView = cc.eyeView;
    // up = cc.d.up;
    worldUp = cc.d.up;
    // front = cc.d.front;
    pitch = cc.d.pitch;
    yaw = cc.d.yaw;
    movementSpeed = FPSCAMERA_MAX_SPEED;

    this->updateVectors();
}

void CameraControllerFPS::onDestroy() {}

void CameraControllerFPS::onUpdate(const double& ts) { this->update(ts); }

void CameraControllerFPS::updateEye() {
    if (eyeView->size() == 1) {
        eyeView->update(glm::lookAt(camera->getPosition(), camera->getPosition() + front, up), camera->getProjection());
    } else {
        glm::vec3 cross1 = glm::cross(up, front);               // up and front already are  vectors!!!!
        glm::vec3 norm1 = glm::normalize(cross1);               // vector side (would be left or right)
        glm::vec3 final_norm1 = norm1 * eyeView->getNoseDist(); // point of eye
        glm::vec3 novaPositionL = camera->getPosition() + final_norm1;
        glm::vec3 novaPositionR = camera->getPosition() - final_norm1;
        eyeView->getHead()[0].update(glm::lookAt(novaPositionL, novaPositionL + front, up), camera->getProjection()); // Left
        eyeView->getHead()[1].update(glm::lookAt(novaPositionR, novaPositionR + front, up), camera->getProjection()); // Right
    }
}

void CameraControllerFPS::updateVectors() {

    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void CameraControllerFPS::processCameraRotation(double xOffset, double yOffset, bool constrainPitch) {
    yaw += (float)xOffset;
    pitch += (float)yOffset;

    // Constrain the pitch
    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        } else if (pitch < -89.0f) {
            pitch = -89.0f;
        }
    }
}

void CameraControllerFPS::processCameraMovement(glm::vec3& direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    // position += direction * velocity;

    camera->setPosition(camera->getPosition() + direction * deltaTime); // FIXME: mover para camera???
}

void CameraControllerFPS::update(const double& ts) {
    // Movement speed
    if (Keyboard::isPressed(SDLK_LSHIFT)) // GLFW_KEY_LEFT_SHIFT
        movementSpeed = FPSCAMERA_MAX_SPEED * 4.0f;
    else if (Keyboard::isPressed(SDLK_LALT)) //  GLFW_KEY_LEFT_ALT
        movementSpeed = FPSCAMERA_MAX_SPEED / 4.0f;
    else
        movementSpeed = FPSCAMERA_MAX_SPEED;

    // CameraFPS movement
    glm::vec3 direction = glm::vec3(0.0f);
    if (Keyboard::isPressed(SDLK_w)) // GLFW_KEY_W
        direction += front;
    if (Keyboard::isPressed(SDLK_s)) // GLFW_KEY_S
        direction -= front;
    if (Keyboard::isPressed(SDLK_a)) // GLFW_KEY_A
        direction -= right;
    if (Keyboard::isPressed(SDLK_d)) //  GLFW_KEY_D
        direction += right;
    if (Keyboard::isPressed(SDLK_SPACE)) // GLFW_KEY_SPACE
        direction += worldUp;
    if (Keyboard::isPressed(SDLK_LCTRL)) //  GLFW_KEY_LEFT_CONTROL
        direction -= worldUp;
    // #if 0 // Temporary controls for controller to test controller data
    // 		direction += front * JoystickManager::GetLeftStick(0).y * 2.0f;
    // 		direction += right * JoystickManager::GetLeftStick(0).x * 2.0f;
    // 		direction += worldUp * JoystickManager::GetTriggers(0).y;
    // 		direction -= worldUp * JoystickManager::GetTriggers(0).x;
    // #endif
    processCameraMovement(direction, ts);

    // CameraFPS FOV : TODO: Alterar o FOV aqui!!!!!
    // float scrollDelta = glm::clamp((float)(InputManager::GetScrollYDelta() * 4.0 + (JoystickManager::GetButton(0, ARCANE_GAMEPAD_A) -
    //                                                                                 JoystickManager::GetButton(0, ARCANE_GAMEPAD_B)
    //                                                                                 * 2.0)),
    //                                -4.0f, 4.0f);
    // ProcessCameraFOV(scrollDelta);

    // CameraFPS rotation
    glm::ivec2 mouseMove = MouseDevice::getMoveRel(); // FIXME: mudar metodo para vec2
    float mouseXDelta = -(float)mouseMove.x * FPSCAMERA_ROTATION_SENSITIVITY;
    float mouseYDelta = (float)mouseMove.y * FPSCAMERA_ROTATION_SENSITIVITY;

    processCameraRotation(mouseXDelta, mouseYDelta, true);
    updateVectors();
    this->updateEye();
}

void CameraControllerFPS::invertPitch() {
    pitch = -pitch;
    updateVectors();
}

// TODO: Mover para a classe de camera!!!!
//  void CameraControllerFPS::processCameraFOV(double offset) {

//     if (offset != 0.0 && fov >= 1.0 && fov <= CAMERA_MAX_FOV) {
//         fov -= (float)offset;
//     }
//     if (fov < 1.0f) {
//         fov = 1.0f;
//     } else if (fov > CAMERA_MAX_FOV) {
//         fov = CAMERA_MAX_FOV;
//     }
// }

//-----

void CameraControllerOrbit::onCreate() {

    // if (cc.d.camKind==CamKind::FPS) // FIXME: Fazer antes acima
    auto& cc = getComponent<CameraComponent>();
    camera = cc.camera; // FIXME: na criacao de camera se define se Perspective ou OrthogonaleyeView->
    eyeView = cc.eyeView;
    up = cc.d.up;
    pitch = cc.d.pitch;
    yaw = cc.d.yaw;
    min = cc.d.min;
    max = cc.d.max;
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