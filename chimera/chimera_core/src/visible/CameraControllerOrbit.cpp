#include "chimera_core/visible/CameraControllerOrbit.hpp"
#include "chimera_base/Mouse.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include <glm/geometric.hpp>

namespace ce {

    CameraControllerOrbit::CameraControllerOrbit(std::shared_ptr<entt::registry> registry, Entity entity)
        : entity_(entity), registry_(registry) {

        this->vp_ = registry->ctx().get<std::shared_ptr<ViewProjection>>();
        this->input_manager_ = registry->ctx().get<std::shared_ptr<InputManager>>();
    }

    CameraControllerOrbit::~CameraControllerOrbit() {}

    void CameraControllerOrbit::on_attach() {
        auto& cc = entity_.getComponent<CameraComponent>(registry_.get());
        camera_ = cc.camera;
        up_ = cc.up;
        // pitch = cc.pitch;
        // yaw = cc.yaw;
        min_ = cc.min;
        max_ = cc.max;
        front_ = {0.0F, 0.0F, 0.0F}; // TODO: melhorar!!
        distance_ = glm::distance(camera_->get_position(), this->front_);

        glm::vec3 direction = glm::normalize(camera_->get_position() - front_);
        pitch_ = glm::degrees(std::asin(direction.y));
        yaw_ = glm::degrees(std::atan2(direction.z, direction.x));

        cc.pitch = pitch_;
        cc.yaw = yaw_;

        this->update_vectors();
    }

    void CameraControllerOrbit::on_deatach() {}

    void CameraControllerOrbit::update_vp() {
        if (vp_->get_size() == 1) {
            vp_->get_left().update(glm::lookAt(camera_->get_position(), front_, up_), camera_->get_projection());
        } else {

            glm::vec3 direcao = glm::normalize(front_ - camera_->get_position());
            const glm::vec3 direita = glm::normalize(glm::cross(direcao, up_));

            const float distancia = vp_->get_noze();
            const glm::vec3 deslocamento = direita * distancia;

            glm::vec3 pos_direita = camera_->get_position() + deslocamento;
            glm::vec3 origem_direita = front_ + deslocamento;

            glm::vec3 pos_esquerda = camera_->get_position() - deslocamento;
            glm::vec3 origem_esquerda = front_ - deslocamento;

            vp_->get_left().update(glm::lookAt(pos_esquerda, origem_esquerda, up_), camera_->get_projection()); // Left
            vp_->get_right().update(glm::lookAt(pos_direita, origem_direita, up_), camera_->get_projection());  // Right

            // const glm::vec3 left_p = front - camera->getPosition(); // front and position as points
            // const glm::vec3 cross1 = glm::cross(up, left_p);
            // const glm::vec3 norm1 = glm::normalize(cross1);
            // const glm::vec3 final_norm1 = norm1 * vp->getNoze();

            // const glm::vec3 novaPositionL = camera->getPosition() + final_norm1;
            // const glm::vec3 novaFrontL = front + final_norm1;
            // vp->getLeft().update(glm::lookAt(posEsquerda, novaFrontL, up), camera->getProjection()); // Left

            // const glm::vec3 novaPositionR = camera->getPosition() - final_norm1;
            // const glm::vec3 novaFrontR = front - final_norm1;
            // vp->getRight().update(glm::lookAt(posDireita, novaFrontR, up), camera->getProjection()); // Right
        }
    }

    void CameraControllerOrbit::update_vectors() {

        const float theta = glm::radians(yaw_); // yaw * 0.017453293f; ( yaw * (PI/180) )
        const float phi = glm::radians(pitch_); // pitch * 0.017453293f;
        glm::vec3 pos;
        if (this->up_.y == 1) {
            pos.x = distance_ * static_cast<float>(sin(phi) * sin(theta));
            pos.y = distance_ * static_cast<float>(cos(phi));
            pos.z = distance_ * static_cast<float>(sin(phi) * cos(theta));
        } else { // this->up.z == 1 ou -1
            pos.x = distance_ * static_cast<float>(cos(theta) * sin(phi));
            pos.y = distance_ * static_cast<float>(cos(theta) * cos(phi));
            pos.z = distance_ * static_cast<float>(sin(theta));
        }

        camera_->set_position(pos);
    }

    void CameraControllerOrbit::process_distance(const int& mz) {

        distance_ += static_cast<float>(mz);

        if (distance_ < min_) { // NOLINT
            distance_ = min_;
        }

        if (distance_ > max_) { // NOLINT
            distance_ = max_;
        }
    }

    void CameraControllerOrbit::process_camera_rotation(const int& x_offset, const int& y_offset,
                                                        bool constrain_pitch) {

        if (this->up_.y == 1) {
            yaw_ -= (float)x_offset;
            pitch_ -= (float)y_offset;

            // Constrain the pitch
            if (constrain_pitch) {
                if (pitch_ < 1.0F) { // NOLINT
                    pitch_ = 1.0F;
                }

                if (pitch_ > 179.0F) { // NOLINT
                    pitch_ = 179.0F;
                }
            }

        } else { // this->->up.z == 1 ou -1

            yaw_ += (float)y_offset;
            pitch_ += (float)x_offset;
            // if (yaw < 1.0f)
            //     yaw = 1.0f;
            // if (yaw > 179.0f)
            //     yaw = 179.0f;
        }
    }

    void CameraControllerOrbit::on_update(const double& ts) {

        if (input_manager_->get_mouse()->is_button_down(Mouse::MouseButton::Left)) {

            const glm::ivec2 mouse_move = input_manager_->get_mouse()->get_delta_xy();
            this->process_camera_rotation(mouse_move.x, mouse_move.y);

        } else if (input_manager_->get_mouse()->is_button_down(Mouse::MouseButton::Right)) {

            const glm::ivec2 mouse_move = input_manager_->get_mouse()->get_delta_xy();
            this->process_distance(mouse_move.y);
        }

        this->update_vectors();
        this->update_vp();
    }

    void CameraControllerOrbit::invert_pitch() {
        pitch_ = -pitch_;
        this->update_vectors();
    }

} // namespace ce
