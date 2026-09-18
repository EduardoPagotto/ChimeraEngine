#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_base/aux/ICamera.hpp"
#include "chimera_base/aux/ViewProjection.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {

    class CameraControllerFPS : public IStateMachine {
      public:
        CameraControllerFPS(std::shared_ptr<entt::registry> registry, Entity entity);
        virtual ~CameraControllerFPS();
        void onAttach() override;
        void onDeatach() override;
        void onRender() override {}
        void onUpdate(const double& ts) override;
        void onEvent(const SDL_Event& event) override {}
        std::string getName() const override { return "CameraControllerFPS"; }

      private:
        void updateVP();
        void updateVectors();
        void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
        void processCameraMovement(glm::vec3& direction, float deltaTime);
        void invertPitch();
        void processCameraFOV(const float& offset);

        float pitch, yaw, movementSpeed;
        glm::vec3 up, front, worldUp, right;
        Entity entity;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<ViewProjection> vp;
        std::shared_ptr<entt::registry> registry;
        std::shared_ptr<InputManager> inputManager;

        ce::Gamepad::AxixConfig player0Config{0.18F, 0.18F, 0.18F}; // Deadzones customizadas
    };
} // namespace ce
