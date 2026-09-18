#include "chimera_core/visible/CameraControllerFPS.hpp"
#include "chimera_base/GamePad.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include <SDL3/SDL_gamepad.h>

namespace ce {

    CameraControllerFPS::CameraControllerFPS(std::shared_ptr<entt::registry> registry, Entity entity)
        : entity(entity), registry(registry) {

        this->inputManager = registry->ctx().get<std::shared_ptr<InputManager>>();
        this->vp = registry->ctx().get<std::shared_ptr<ViewProjection>>();
    }

    CameraControllerFPS::~CameraControllerFPS() {}

    void CameraControllerFPS::onAttach() {

        auto& cc = entity.getComponent<CameraComponent>();
        camera = cc.camera;
        up = cc.up;
        worldUp = cc.up;
        pitch = cc.pitch;
        yaw = cc.yaw;
        movementSpeed = fsp_camera_max_speed;

        this->updateVectors();
    }

    void CameraControllerFPS::onDeatach() {}

    void CameraControllerFPS::updateVP() {
        if (vp->getSize() == 1) {
            vp->getLeft().update(glm::lookAt(camera->getPosition(), camera->getPosition() + front, up),
                                 camera->getProjection());
        } else {
            glm::vec3 cross1 = glm::cross(up, front);      // up and front already are  vectors!!!!
            glm::vec3 norm1 = glm::normalize(cross1);      // vector side (would be left or right)
            glm::vec3 final_norm1 = norm1 * vp->getNoze(); // point of eye
            glm::vec3 novaPositionL = camera->getPosition() + final_norm1;
            glm::vec3 novaPositionR = camera->getPosition() - final_norm1;
            vp->getLeft().update(glm::lookAt(novaPositionL, novaPositionL + front, up),
                                 camera->getProjection()); // Left
            vp->getRight().update(glm::lookAt(novaPositionR, novaPositionR + front, up),
                                  camera->getProjection()); // Right
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
            if (pitch > 89.0F) {
                pitch = 89.0F;
            } else if (pitch < -89.0F) {
                pitch = -89.0F;
            }
        }
    }

    void CameraControllerFPS::processCameraMovement(glm::vec3& direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        camera->setPosition(camera->getPosition() + direction * velocity);
    }

    void CameraControllerFPS::onUpdate(const double& ts) {
        // Movement speed
        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_LSHIFT)) { // acelerar mover

            movementSpeed = fsp_camera_max_speed * 4.0F;
        } else if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_LALT)) { //  desacelerar mover

            movementSpeed = fsp_camera_max_speed / 4.0F;
        } else {

            movementSpeed = fsp_camera_max_speed;
        }

        // CameraFPS movement
        glm::vec3 direction = glm::vec3(0.0F);
        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_W)) { // to foward
            direction += front;
        }

        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_S)) { // to backward
            direction -= front;
        }

        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_A)) { // to left
            direction -= right;
        }

        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_D)) { //  to right
            direction += right;
        }

        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_SPACE)) { // to up
            direction += worldUp;
        }

        if (inputManager->getKeyboard()->isKeyDown(SDL_SCANCODE_LCTRL)) { //  to booton
            direction -= worldUp;
        }

        float mouseXDelta{0.0F};
        float mouseYDelta{0.0F};

        auto gp = this->inputManager->getGamepad();
        auto ms = this->inputManager->getMouse();

        glm::vec2 leftStick = gp->getLeftStick(0, player0Config);
        if (glm::length(leftStick) > 0.0F) {

            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Esquerdo -> X: %f | Y: %f", leftStick.x,
                         leftStick.y);

            direction += front * leftStick.y * 1.5F; // mov FB
            direction -= right * leftStick.x * 1.5F; // mov RL
        }

        glm::vec2 rightStick = gp->getRightStick(0, player0Config);
        if (glm::length(rightStick) > 0.0F) {

            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Direito -> X: %f | Y: %f", rightStick.x,
                         rightStick.y);

            mouseXDelta = -rightStick.x * 1.5F; // rot RL
            mouseYDelta = rightStick.y * 1.5F;  // rot UD
        } else {
            // Mouse Camera rotation
            glm::ivec2 mouseMove = ms->getDeltaXY(); //  ->getMoveRel();
            mouseXDelta = -(float)mouseMove.x * fsp_camera_rotation_sensitivity;
            mouseYDelta = (float)mouseMove.y * fsp_camera_rotation_sensitivity;
        }

        Gamepad::ButtonState pad_up = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_DPAD_UP);
        if (pad_up == Gamepad::ButtonState::Pressed || pad_up == Gamepad::ButtonState::Held) {
            direction += (worldUp * 0.5F); // mov U<->D
        }

        Gamepad::ButtonState pad_down = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
        if (pad_down == Gamepad::ButtonState::Pressed || pad_down == Gamepad::ButtonState::Held) {
            direction -= worldUp * 0.5F; // mov D<->U
        }

        glm::vec2 triggerStick = gp->getTriggerStick(0, player0Config);
        if (glm::length(triggerStick) > 0.0F) {

            // Gamepad::ButtonState north = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_NORTH);
            // Gamepad::ButtonState south = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_SOUTH);

            // axis16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFT_TRIGGER), deadZone, 0x8000);
            const float v1 = triggerStick.x;
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, " V1: %f", v1);

            // if (north == Gamepad::ButtonState::Pressed) {

            //     const float v2 = v1 * 4.0;
            //     const float scrollDelta = glm::clamp(v2 * 4.0F, -4.0F, 4.0F);
            //     processCameraFOV(scrollDelta); // TODO: injetar o novo FOV na camera, passar ele para perspective
            // }

            // if (south == Gamepad::ButtonState::Pressed) {

            //     const float v2 = -v1 * 4.0;
            //     const float scrollDelta = glm::clamp(v2 * 4.0F, -4.0F, 4.0F);
            //     processCameraFOV(scrollDelta); // TODO: injetar o novo FOV na camera, passar ele para perspective
            // }
        }

        processCameraMovement(direction, ts);

        processCameraRotation(mouseXDelta, mouseYDelta, true);
        updateVectors();
        this->updateVP();
    }

    void CameraControllerFPS::invertPitch() {
        pitch = -pitch;
        updateVectors();
    }

    // TODO: Mover para a classe de camera!!!!
    void CameraControllerFPS::processCameraFOV(const float& offset) {

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, " FOV: %f", offset);

        // if (offset != 0.0 && fov >= 1.0 && fov <= camera_max_fov) {
        //     fov -= (float)offset;
        // }
        // if (fov < 1.0f) {
        //     fov = 1.0f;
        // } else if (fov > camera_max_fov) {
        //     fov = camera_max_fov;
        // }
    }
} // namespace ce
