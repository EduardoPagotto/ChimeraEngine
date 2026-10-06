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
        virtual ~CameraControllerFPS() = default;
        void on_attach() override;
        void on_deatach() override;
        void on_render() override {}
        void on_update(const double& ts) override;
        void on_event(const SDL_Event& event) override {}
        std::string get_name() const override { return "CameraControllerFPS"; }

      private:
        void update_vp();
        void update_vectors();
        void process_camera_rotation(double x_offset, double y_offset, bool constrain_pitch);
        void process_camera_movement(glm::vec3& direction, float delta_time);
        void invert_pitch();
        void process_camera_fov(const float& offset);

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
