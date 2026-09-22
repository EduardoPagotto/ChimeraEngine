#include "chimera_core/visible/CameraControllerOrbit.hpp"
#include "chimera_base/Mouse.hpp"
#include "chimera_ecs/CameraComponent.hpp"
#include <glm/geometric.hpp>

namespace ce {

    CameraControllerOrbit::CameraControllerOrbit(std::shared_ptr<entt::registry> registry, Entity entity)
        : entity(entity), registry(registry) {

        this->vp = registry->ctx().get<std::shared_ptr<ViewProjection>>();
        this->inputManager = registry->ctx().get<std::shared_ptr<InputManager>>();
    }

    CameraControllerOrbit::~CameraControllerOrbit() {}

    void CameraControllerOrbit::onAttach() {
        auto& cc = entity.getComponent<CameraComponent>(registry.get());
        camera = cc.camera;
        up = cc.up;
        // pitch = cc.pitch;
        // yaw = cc.yaw;
        min = cc.min;
        max = cc.max;
        front = {0.0F, 0.0F, 0.0F}; // TODO: melhorar!!
        distance = glm::distance(camera->getPosition(), this->front);

        glm::vec3 direction = glm::normalize(camera->getPosition() - front);
        pitch = glm::degrees(std::asin(direction.y));
        yaw = glm::degrees(std::atan2(direction.z, direction.x));

        cc.pitch = pitch;
        cc.yaw = yaw;

        this->updateVectors();
    }

    void CameraControllerOrbit::onDeatach() {}

    void CameraControllerOrbit::updateVP() {
        if (vp->getSize() == 1) {
            vp->getLeft().update(glm::lookAt(camera->getPosition(), front, up), camera->getProjection());
        } else {

            glm::vec3 direcao = glm::normalize(front - camera->getPosition());
            const glm::vec3 direita = glm::normalize(glm::cross(direcao, up));

            const float distancia = vp->getNoze();
            const glm::vec3 deslocamento = direita * distancia;

            glm::vec3 posDireita = camera->getPosition() + deslocamento;
            glm::vec3 origemDireita = front + deslocamento;

            glm::vec3 posEsquerda = camera->getPosition() - deslocamento;
            glm::vec3 origemEsquerda = front - deslocamento;

            vp->getLeft().update(glm::lookAt(posEsquerda, origemEsquerda, up), camera->getProjection()); // Left
            vp->getRight().update(glm::lookAt(posDireita, origemDireita, up), camera->getProjection());  // Right

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

        const float theta = glm::radians(yaw); // yaw * 0.017453293f; ( yaw * (PI/180) )
        const float phi = glm::radians(pitch); // pitch * 0.017453293f;
        glm::vec3 pos;
        if (this->up.y == 1) {
            pos.x = distance * static_cast<float>(sin(phi) * sin(theta));
            pos.y = distance * static_cast<float>(cos(phi));
            pos.z = distance * static_cast<float>(sin(phi) * cos(theta));
        } else { // this->up.z == 1 ou -1
            pos.x = distance * static_cast<float>(cos(theta) * sin(phi));
            pos.y = distance * static_cast<float>(cos(theta) * cos(phi));
            pos.z = distance * static_cast<float>(sin(theta));
        }

        camera->setPosition(pos);
    }

    void CameraControllerOrbit::processDistance(const int& _mz) {

        distance += static_cast<float>(_mz);

        if (distance < min) { // NOLINT
            distance = min;
        }

        if (distance > max) { // NOLINT
            distance = max;
        }
    }

    void CameraControllerOrbit::processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch) {

        if (this->up.y == 1) {
            yaw -= (float)xOffset;
            pitch -= (float)yOffset;

            // Constrain the pitch
            if (constrainPitch) {
                if (pitch < 1.0F) { // NOLINT
                    pitch = 1.0F;
                }

                if (pitch > 179.0F) { // NOLINT
                    pitch = 179.0F;
                }
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

    void CameraControllerOrbit::onUpdate(const double& ts) {

        if (inputManager->getMouse()->isButtonDown(Mouse::MouseButton::Left)) {

            const glm::ivec2 mouseMove = inputManager->getMouse()->getDeltaXY();
            this->processCameraRotation(mouseMove.x, mouseMove.y);

        } else if (inputManager->getMouse()->isButtonDown(Mouse::MouseButton::Right)) {

            const glm::ivec2 mouseMove = inputManager->getMouse()->getDeltaXY();
            this->processDistance(mouseMove.y);
        }

        this->updateVectors();
        this->updateVP();
    }

    void CameraControllerOrbit::invertPitch() {
        pitch = -pitch;
        this->updateVectors();
    }

} // namespace ce
