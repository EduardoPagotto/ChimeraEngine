#include "chimera_core/visible/CameraControllerFPS.hpp"
#include "chimera_base/GamePad.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include <SDL3/SDL_gamepad.h>

namespace ce {

    CameraControllerFPS::CameraControllerFPS(std::shared_ptr<entt::registry> registry, Entity entity)
        : entity_(entity), registry_(registry) {

        this->input_manager_ = registry->ctx().get<std::shared_ptr<InputManager>>();
        this->vp_ = registry->ctx().get<std::shared_ptr<ViewProjection>>();
    }

    CameraControllerFPS::~CameraControllerFPS() {}

    void CameraControllerFPS::on_attach() {

        auto& cc = entity_.getComponent<CameraComponent>(registry_.get());
        camera_ = cc.camera;
        up_ = cc.up;
        world_up_ = cc.up;
        pitch_ = cc.pitch;
        yaw_ = cc.yaw;
        movement_speed_ = fsp_camera_max_speed;

        this->update_vectors();
    }

    void CameraControllerFPS::on_deatach() {}

    void CameraControllerFPS::update_vp() {
        if (vp_->get_size() == 1) {
            vp_->get_left().update(glm::lookAt(camera_->get_position(), camera_->get_position() + front_, up_),
                                   camera_->get_projection());
        } else {
            glm::vec3 cross1 = glm::cross(up_, front_);      // up and front already are  vectors!!!!
            glm::vec3 norm1 = glm::normalize(cross1);        // vector side (would be left or right)
            glm::vec3 final_norm1 = norm1 * vp_->get_noze(); // point of eye
            glm::vec3 nova_position_l = camera_->get_position() + final_norm1;
            glm::vec3 nova_position_r = camera_->get_position() - final_norm1;
            vp_->get_left().update(glm::lookAt(nova_position_l, nova_position_l + front_, up_),
                                   camera_->get_projection()); // Left
            vp_->get_right().update(glm::lookAt(nova_position_r, nova_position_r + front_, up_),
                                    camera_->get_projection()); // Right
        }
    }

    void CameraControllerFPS::update_vectors() {

        front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_.y = sin(glm::radians(pitch_));
        front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_ = glm::normalize(front_);

        right_ = glm::normalize(glm::cross(front_, world_up_));
        up_ = glm::normalize(glm::cross(right_, front_));
    }

    void CameraControllerFPS::process_camera_rotation(double x_offset, double y_offset, bool constrain_pitch) {
        yaw_ += (float)x_offset;
        pitch_ += (float)y_offset;

        // Constrain the pitch
        if (constrain_pitch) {
            if (pitch_ > 89.0F) {
                pitch_ = 89.0F;
            } else if (pitch_ < -89.0F) {
                pitch_ = -89.0F;
            }
        }
    }

    void CameraControllerFPS::process_camera_movement(glm::vec3& direction, float delta_time) {
        float velocity = movement_speed_ * delta_time;
        camera_->set_position(camera_->get_position() + direction * velocity);
    }

    void CameraControllerFPS::on_update(const double& ts) {
        // Movement speed
        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_LSHIFT)) { // acelerar mover

            movement_speed_ = fsp_camera_max_speed * 4.0F;
        } else if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_LALT)) { //  desacelerar mover

            movement_speed_ = fsp_camera_max_speed / 4.0F;
        } else {

            movement_speed_ = fsp_camera_max_speed;
        }

        // CameraFPS movement
        glm::vec3 direction = glm::vec3(0.0F);
        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_W)) { // to foward
            direction += front_;
        }

        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_S)) { // to backward
            direction -= front_;
        }

        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_A)) { // to left
            direction -= right_;
        }

        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_D)) { //  to right
            direction += right_;
        }

        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_SPACE)) { // to up
            direction += world_up_;
        }

        if (input_manager_->get_keyboard()->is_key_down(SDL_SCANCODE_LCTRL)) { //  to booton
            direction -= world_up_;
        }

        float mouse_x_delta{0.0F};
        float mouse_y_delta{0.0F};

        auto gp = this->input_manager_->get_gamepad();
        auto ms = this->input_manager_->get_mouse();

        glm::vec2 left_stick = gp->get_left_stick(0, player0_config_);
        if (glm::length(left_stick) > 0.0F) {

            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Esquerdo -> X: %f | Y: %f", left_stick.x,
                         left_stick.y);

            direction += front_ * left_stick.y * 1.5F; // mov FB
            direction -= right_ * left_stick.x * 1.5F; // mov RL
        }

        glm::vec2 right_stick = gp->get_right_stick(0, player0_config_);
        if (glm::length(right_stick) > 0.0F) {

            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "[Player 0] Movendo Stick Direito -> X: %f | Y: %f", right_stick.x,
                         right_stick.y);

            mouse_x_delta = -right_stick.x * 1.5F; // rot RL
            mouse_y_delta = right_stick.y * 1.5F;  // rot UD
        } else {
            // Mouse Camera rotation
            glm::ivec2 mouse_move = ms->get_delta_xy(); //  ->getMoveRel();
            mouse_x_delta = -(float)mouse_move.x * fsp_camera_rotation_sensitivity;
            mouse_y_delta = (float)mouse_move.y * fsp_camera_rotation_sensitivity;
        }

        Gamepad::ButtonState pad_up = gp->get_button_state(0, SDL_GAMEPAD_BUTTON_DPAD_UP);
        if (pad_up == Gamepad::ButtonState::Pressed || pad_up == Gamepad::ButtonState::Held) {
            direction += (world_up_ * 0.5F); // mov U<->D
        }

        Gamepad::ButtonState pad_down = gp->get_button_state(0, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
        if (pad_down == Gamepad::ButtonState::Pressed || pad_down == Gamepad::ButtonState::Held) {
            direction -= world_up_ * 0.5F; // mov D<->U
        }

        glm::vec2 trigger_stick = gp->get_trigger_stick(0, player0_config_);
        if (glm::length(trigger_stick) > 0.0F) {

            // Gamepad::ButtonState north = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_NORTH);
            // Gamepad::ButtonState south = gp->getButtonState(0, SDL_GAMEPAD_BUTTON_SOUTH);

            // axis16(SDL_GetGamepadAxis(pJoy, SDL_GAMEPAD_AXIS_LEFT_TRIGGER), deadZone, 0x8000);
            const float v1 = trigger_stick.x;
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

        process_camera_movement(direction, ts);

        process_camera_rotation(mouse_x_delta, mouse_y_delta, true);
        update_vectors();
        this->update_vp();
    }

    void CameraControllerFPS::invert_pitch() {
        pitch_ = -pitch_;
        update_vectors();
    }

    // TODO: Mover para a classe de camera!!!!
    void CameraControllerFPS::process_camera_fov(const float& offset) {

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
