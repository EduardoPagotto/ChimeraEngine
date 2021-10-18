#include "chimera/render/scene/CameraControllerFPS.hpp"
#include "chimera/core/io/Keyboard.hpp"
#include "chimera/core/io/MouseDevice.hpp"

namespace Chimera {

void CameraControllerFPS::onCreate() {

    cp = &getComponent<CameraControlerFPSParams>();

    this->worldUp = cp->up;
    // this->movementSpeed = FPSCAMERA_MAX_SPEED;

    auto& cc = getComponent<CameraComponent>();
    camera = &cc.camera;

    this->updateVectors();
}

void CameraControllerFPS::onDestroy() {}

void CameraControllerFPS::recalculateMatrix(bool left) { // windows x->width; y -> height

    glm::mat4 viewMatrix;

    if (left == false) {
        viewMatrix = glm::lookAt(camera->getPosition(), camera->getPosition() + cp->front, cp->up);

    } else {
        glm::vec3 novaPosition = camera->getPosition();
        glm::vec3 novaFront = cp->front;
        glm::vec3 left_p = novaFront - novaPosition;
        glm::vec3 cross1 = glm::cross(cp->up, left_p);
        glm::vec3 norm1 = glm::normalize(cross1);
        glm::vec3 final_norm1 = norm1 * 5.0f;

        novaPosition = camera->getPosition() - final_norm1;
        novaFront = cp->front - final_norm1;

        viewMatrix = glm::lookAt(novaPosition, novaPosition + novaFront, cp->up);
    }

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(cp->fov), camera->getAspectRatio(), camera->getNear(), camera->getFar());

    camera->updateAllMatrix(projectionMatrix, viewMatrix);
}

void CameraControllerFPS::invertPitch() {
    cp->pitch = -cp->pitch;
    updateVectors();
}

void CameraControllerFPS::updateVectors() {

    cp->front.x = cos(glm::radians(cp->yaw)) * cos(glm::radians(cp->pitch));
    cp->front.y = sin(glm::radians(cp->pitch));
    cp->front.z = sin(glm::radians(cp->yaw)) * cos(glm::radians(cp->pitch));
    cp->front = glm::normalize(cp->front);

    right = glm::normalize(glm::cross(cp->front, worldUp));
    cp->up = glm::normalize(glm::cross(right, cp->front));
}

void CameraControllerFPS::onUpdate(float deltaTime) {
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
        direction += cp->front;
    if (Keyboard::isPressed(SDLK_s)) // GLFW_KEY_S
        direction -= cp->front;
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
    processCameraMovement(direction, deltaTime);

    // CameraFPS FOV
    // float scrollDelta = glm::clamp((float)(InputManager::GetScrollYDelta() * 4.0 + (JoystickManager::GetButton(0, ARCANE_GAMEPAD_A) -
    //                                                                                 JoystickManager::GetButton(0, ARCANE_GAMEPAD_B)
    //                                                                                 * 2.0)),
    //                                -4.0f, 4.0f);
    // ProcessCameraFOV(scrollDelta);

    // CameraFPS rotation
    // float mouseXDelta =
    //     (float)InputManager::GetMouseXDelta() /*+ ((float)JoystickManager::GetRightStick(0).x * 20.0)*/ *
    // FPSCAMERA_ROTATION_SENSITIVITY;
    // float mouseYDelta =
    //     (float)-InputManager::GetMouseYDelta() /*+ ((float)JoystickManager::GetRightStick(0).y * 20.0)*/ *
    //     FPSCAMERA_ROTATION_SENSITIVITY;

    glm::ivec2 mouseMove = MouseDevice::getMoveRel(); // FIXME: mudar metodo para vec2
    float mouseXDelta = -(float)mouseMove.x * FPSCAMERA_ROTATION_SENSITIVITY;
    float mouseYDelta = (float)mouseMove.y * FPSCAMERA_ROTATION_SENSITIVITY;

    processCameraRotation(mouseXDelta, mouseYDelta, true);

    // TODO: testar se direction e mouseMove != 0 para executar updates
    updateVectors();
    recalculateMatrix(false);
}

void CameraControllerFPS::processCameraMovement(glm::vec3& direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    camera->incrementPosition(direction * velocity); // position += direction * velocity;
}

void CameraControllerFPS::processCameraRotation(double xOffset, double yOffset, bool constrainPitch) {

    cp->yaw += (float)xOffset;
    cp->pitch += (float)yOffset;

    // Constrain the pitch
    if (constrainPitch) {
        if (cp->pitch > 89.0f) {
            cp->pitch = 89.0f;
        } else if (cp->pitch < -89.0f) {
            cp->pitch = -89.0f;
        }
    }
}

void CameraControllerFPS::processCameraFOV(double offset) {

    if (offset != 0.0 && cp->fov >= 1.0 && cp->fov <= CAMERA_MAX_FOV) {
        cp->fov -= (float)offset;
    }
    if (cp->fov < 1.0f) {
        cp->fov = 1.0f;
    } else if (cp->fov > CAMERA_MAX_FOV) {
        cp->fov = CAMERA_MAX_FOV;
    }
}

} // namespace Chimera