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
        distance_ = glm::distance(camera_->getPosition(), this->front_);

        glm::vec3 direction = glm::normalize(camera_->getPosition() - front_);
        pitch_ = glm::degrees(std::asin(direction.y));
        yaw_ = glm::degrees(std::atan2(direction.z, direction.x));

        cc.pitch = pitch_;
        cc.yaw = yaw_;

        this->updateVectors();
    }

    void CameraControllerOrbit::on_deatach() {}

    void CameraControllerOrbit::updateVP() {
        if (vp_->getSize() == 1) {
            vp_->getLeft().update(glm::lookAt(camera_->getPosition(), front_, up_), camera_->getProjection());
        } else {

            glm::vec3 direcao = glm::normalize(front_ - camera_->getPosition());
            const glm::vec3 direita = glm::normalize(glm::cross(direcao, up_));

            const float distancia = vp_->getNoze();
            const glm::vec3 deslocamento = direita * distancia;

            glm::vec3 posDireita = camera_->getPosition() + deslocamento;
            glm::vec3 origemDireita = front_ + deslocamento;

            glm::vec3 posEsquerda = camera_->getPosition() - deslocamento;
            glm::vec3 origemEsquerda = front_ - deslocamento;

            vp_->getLeft().update(glm::lookAt(posEsquerda, origemEsquerda, up_), camera_->getProjection()); // Left
            vp_->getRight().update(glm::lookAt(posDireita, origemDireita, up_), camera_->getProjection());  // Right

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

    void CameraControllerOrbit::updateVectors() {

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

        camera_->setPosition(pos);
    }

    void CameraControllerOrbit::processDistance(const int& _mz) {

        distance_ += static_cast<float>(_mz);

        if (distance_ < min_) { // NOLINT
            distance_ = min_;
        }

        if (distance_ > max_) { // NOLINT
            distance_ = max_;
        }
    }

    void CameraControllerOrbit::processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch) {

        if (this->up_.y == 1) {
            yaw_ -= (float)xOffset;
            pitch_ -= (float)yOffset;

            // Constrain the pitch
            if (constrainPitch) {
                if (pitch_ < 1.0F) { // NOLINT
                    pitch_ = 1.0F;
                }

                if (pitch_ > 179.0F) { // NOLINT
                    pitch_ = 179.0F;
                }
            }

        } else { // this->->up.z == 1 ou -1

            yaw_ += (float)yOffset;
            pitch_ += (float)xOffset;
            // if (yaw < 1.0f)
            //     yaw = 1.0f;
            // if (yaw > 179.0f)
            //     yaw = 179.0f;
        }
    }

    void CameraControllerOrbit::on_update(const double& ts) {

        if (input_manager_->getMouse()->isButtonDown(Mouse::MouseButton::Left)) {

            const glm::ivec2 mouseMove = input_manager_->getMouse()->getDeltaXY();
            this->processCameraRotation(mouseMove.x, mouseMove.y);

        } else if (input_manager_->getMouse()->isButtonDown(Mouse::MouseButton::Right)) {

            const glm::ivec2 mouseMove = input_manager_->getMouse()->getDeltaXY();
            this->processDistance(mouseMove.y);
        }

        this->updateVectors();
        this->updateVP();
    }

    void CameraControllerOrbit::invertPitch() {
        pitch_ = -pitch_;
        this->updateVectors();
    }

} // namespace ce
