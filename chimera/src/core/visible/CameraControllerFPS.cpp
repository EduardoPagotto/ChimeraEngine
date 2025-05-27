#include "chimera/core/visible/CameraControllerFPS.hpp"
#include "chimera/base/utils.hpp"
#include "chimera/ecs/CameraComponent.hpp"

namespace ce {

    CameraControllerFPS::CameraControllerFPS(Entity entity) : IStateMachine("FPS"), entity(entity) {

        vp = g_service_locator.getService<ViewProjection>();
        mouse = g_service_locator.getService<Mouse>();
        keyboard = g_service_locator.getService<Keyboard>();
        gameControl = g_service_locator.getService<GamePad>();
    }

    CameraControllerFPS::~CameraControllerFPS() {
        mouse = nullptr;
        keyboard = nullptr;
        gameControl = nullptr;
    }

    void CameraControllerFPS::onAttach() {

        auto& cc = entity.getComponent<CameraComponent>();
        camera = cc.camera;
        up = cc.up;
        worldUp = cc.up;
        pitch = cc.pitch;
        yaw = cc.yaw;
        movementSpeed = FPSCAMERA_MAX_SPEED;

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

    void CameraControllerFPS::onUpdate(const double& ts) {
        // Movement speed
        if (keyboard->isPressed(SDLK_LSHIFT)) // acelerar mover
            movementSpeed = FPSCAMERA_MAX_SPEED * 4.0f;
        else if (keyboard->isPressed(SDLK_LALT)) //  desacelerar mover
            movementSpeed = FPSCAMERA_MAX_SPEED / 4.0f;
        else
            movementSpeed = FPSCAMERA_MAX_SPEED;

        // CameraFPS movement
        glm::vec3 direction = glm::vec3(0.0f);
        if (keyboard->isPressed(SDLK_W)) // to foward
            direction += front;
        if (keyboard->isPressed(SDLK_S)) // to backward
            direction -= front;
        if (keyboard->isPressed(SDLK_A)) // to left
            direction -= right;
        if (keyboard->isPressed(SDLK_D)) //  to right
            direction += right;
        if (keyboard->isPressed(SDLK_SPACE)) // to up
            direction += worldUp;
        if (keyboard->isPressed(SDLK_LCTRL)) //  to booton
            direction -= worldUp;

        float mouseXDelta{0.0f};
        float mouseYDelta{0.0f};

        if (SDL_Gamepad* pJoy = gameControl->get(0); pJoy != nullptr) {
            // Game control ratation and move
            int16_t deadZone = 128;
            direction += front * scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFTY), deadZone), 0x8000) *
                         1.5f; // mov FB

            direction -= right * scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFTX), deadZone), 0x8000) *
                         1.5f; // mov RL

            mouseXDelta =
                -scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_RIGHTX), deadZone), 0x8000) * 1.5f; // rot RL

            mouseYDelta =
                scale16(dead16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_RIGHTY), deadZone), 0x8000) * 1.5f; // rot UD

            if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_DPAD_UP) == true)
                direction += (worldUp * 0.5f); // mov U<->D

            if (SDL_GetGamepadButton(pJoy, SDL_GAMEPAD_BUTTON_DPAD_DOWN) == true)
                direction -= worldUp * 0.5f; // mov D<->U

            // float v1 = scale16(dead16(SDL_GameControllerGetAxis(pJoy, SDL_CONTROLLER_AXIS_TRIGGERLEFT), deadZone),
            // 0x8000); float v2 = v1 * 4.0 + SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_Y) -
            //            SDL_GameControllerGetButton(pJoy, SDL_CONTROLLER_BUTTON_X) * 2.0f;
            // float scrollDelta = glm::clamp(v2 * 4.0f, -4.0f, 4.0f);
            // ProcessCameraFOV(scrollDelta); // TODO: injetar o novo FOV na camera, passar ele para perspective

        } else {
            // Mouse Camera rotation
            glm::ivec2 mouseMove = mouse->getMoveRel();
            mouseXDelta = -(float)mouseMove.x * FPSCAMERA_ROTATION_SENSITIVITY;
            mouseYDelta = (float)mouseMove.y * FPSCAMERA_ROTATION_SENSITIVITY;
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
} // namespace ce
