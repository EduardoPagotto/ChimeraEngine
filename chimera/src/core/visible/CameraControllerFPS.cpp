#include "chimera/core/visible/CameraControllerFPS.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/MouseDevice.hpp"

namespace Chimera {

void CameraControllerFPS::onAttach() {

    auto& cc = entity.getComponent<CameraComponent>();
    camera = cc.camera;
    vpo = cc.vpo;
    up = cc.up;
    worldUp = cc.up;
    pitch = cc.pitch;
    yaw = cc.yaw;
    movementSpeed = FPSCAMERA_MAX_SPEED;

    this->updateVectors();
}

void CameraControllerFPS::onDeatach() {}

void CameraControllerFPS::updateEye() {
    if (vpo->size() == 1) {
        vpo->update(glm::lookAt(camera->getPosition(), camera->getPosition() + front, up), camera->getProjection());
    } else {
        glm::vec3 cross1 = glm::cross(up, front);           // up and front already are  vectors!!!!
        glm::vec3 norm1 = glm::normalize(cross1);           // vector side (would be left or right)
        glm::vec3 final_norm1 = norm1 * vpo->getNoseDist(); // point of eye
        glm::vec3 novaPositionL = camera->getPosition() + final_norm1;
        glm::vec3 novaPositionR = camera->getPosition() - final_norm1;
        vpo->getHead()[0].update(glm::lookAt(novaPositionL, novaPositionL + front, up), camera->getProjection()); // Left
        vpo->getHead()[1].update(glm::lookAt(novaPositionR, novaPositionR + front, up), camera->getProjection()); // Right
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

void CameraControllerFPS::onUpdate(ViewProjection& vp, const double& ts) {
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
} // namespace Chimera