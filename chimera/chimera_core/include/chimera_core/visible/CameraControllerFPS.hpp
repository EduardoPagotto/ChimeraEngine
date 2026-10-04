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
        void on_attach() override;
        void on_deatach() override;
        void on_render() override {}
        void on_update(const double& ts) override;
        void on_event(const SDL_Event& event) override {}
        std::string get_name() const override { return "CameraControllerFPS"; }

      private:
        void updateVP();
        void updateVectors();
        void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
        void processCameraMovement(glm::vec3& direction, float deltaTime);
        void invertPitch();
        void processCameraFOV(const float& offset);

        float pitch_, yaw_, movement_speed_;
        glm::vec3 up_, front_, world_up_, right_;
        Entity entity_;
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<ViewProjection> vp_;
        std::shared_ptr<entt::registry> registry_;
        std::shared_ptr<InputManager> input_manager_;

        ce::Gamepad::AxixConfig player0_config_{0.18F, 0.18F, 0.18F}; // Deadzones customizadas
    };
} // namespace ce
