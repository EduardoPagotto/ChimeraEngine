#include "chimera/core/visible/CameraControllerFPS.hpp"
#include "chimera/core/Singleton.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/utils.hpp"

namespace Chimera {

void CameraControllerFPS::onAttach() {
    gameControl = Singleton<GameController>::get();
    gameControl->init();

    auto& cc = entity.getComponent<CameraComponent>();
    camera = cc.camera;
    up = cc.up;
    worldUp = cc.up;
    pitch = cc.pitch;
    yaw = cc.yaw;
    movementSpeed = FPSCAMERA_MAX_SPEED;

    this->updateVectors();
}

void CameraControllerFPS::onDeatach() { gameControl->release(); }

void CameraControllerFPS::updateVP(ViewProjection& vp) {
    if (vp.size() == 1) {
        vp.update(glm::lookAt(camera->getPosition(), camera->getPosition() + front, up), camera->getProjection());
    } else {
        glm::vec3 cross1 = glm::cross(up, front);         // up and front already are  vectors!!!!
        glm::vec3 norm1 = glm::normalize(cross1);         // vector side (would be left or right)
        glm::vec3 final_norm1 = norm1 * vp.getNoseDist(); // point of eye
        glm::vec3 novaPositionL = camera->getPosition() + final_norm1;
        glm::vec3 novaPositionR = camera->getPosition() - final_norm1;
        vp.getHead()[0].update(glm::lookAt(novaPositionL, novaPositionL + front, up), camera->getProjection()); // Left
        vp.getHead()[1].update(glm::lookAt(novaPositionR, novaPositionR + front, up), camera->getProjection()); // Right
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
    camera->setPosition(camera->getPosition() + direction * velocity);
}

void CameraControllerFPS::onUpdate(ViewProjection& vp, const double& ts) {
    // Movement speed
    if (Keyboard::isPressed(SDLK_LSHIFT)) // acelerar mover
        movementSpeed = FPSCAMERA_MAX_SPEED * 4.0f;
    else if (Keyboard::isPressed(SDLK_LALT)) //  desacelerar mover
        movementSpeed = FPSCAMERA_MAX_SPEED / 4.0f;
    else
        movementSpeed = FPSCAMERA_MAX_SPEED;

    // CameraFPS movement
    glm::vec3 direction = glm::vec3(0.0f);
    if (Keyboard::isPressed(SDLK_w)) // to foward
        direction += front;
    if (Keyboard::isPressed(SDLK_s)) // to backward
        direction -= front;
    if (Keyboard::isPressed(SDLK_a)) // to left
        direction -= right;
    if (Keyboard::isPressed(SDLK_d)) //  to right
        direction += right;
    if (Keyboard::isPressed(SDLK_SPACE)) // to up
        direction += worldUp;
    if (Keyboard::isPressed(SDLK_LCTRL)) //  to booton
        direction -= worldUp;

    float mouseXDelta{0.0f};
    float mouseYDelta{0.0f};

    if (SDL_GameController* pJoy = gameControl->get(0); pJoy != nullptr) {
        // Game control ratation and move
        int16_t deadZone = 128;
        direction += front * scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_LEFTY), deadZone), 0x8000) * 1.5f; // mov FB
        direction -= right * scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_LEFTX), deadZone), 0x8000) * 1.5f; // mov RL

        mouseXDelta = -scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_RIGHTX), deadZone), 0x8000) * 1.5f; // rot RL
        mouseYDelta = scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_RIGHTY), deadZone), 0x8000) * 1.5f;  // rot UD

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_DPAD_UP) == SDL_PRESSED)
            direction += (worldUp * 0.5f); // mov U<->D

        if (SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_DPAD_DOWN) == SDL_PRESSED)
            direction -= worldUp * 0.5f; // mov D<->U

        // float v1 = scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_TRIGGERLEFT), deadZone), 0x8000);
        // float v2 = v1 * 4.0 + SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_Y) -
        //            SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_X) * 2.0f;
        // float scrollDelta = glm::clamp(v2 * 4.0f, -4.0f, 4.0f);
        // ProcessCameraFOV(scrollDelta); // TODO: injetar o novo FOV na camera, passar ele para perspective

    } else {
        // Mouse Camera rotation
        glm::ivec2 mouseMove = MouseDevice::getMoveRel();
        mouseXDelta = -(float)mouseMove.x * FPSCAMERA_ROTATION_SENSITIVITY;
        mouseYDelta = (float)mouseMove.y * FPSCAMERA_ROTATION_SENSITIVITY;
    }

    processCameraMovement(direction, ts);

    processCameraRotation(mouseXDelta, mouseYDelta, true);
    updateVectors();
    this->updateVP(vp);
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
} // namespace Chimera