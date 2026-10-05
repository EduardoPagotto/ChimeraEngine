#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_base/aux/ICamera.hpp"
#include "chimera_base/aux/ViewProjection.hpp"
#include "chimera_ecs/Entity.hpp"

namespace ce {

    class CameraControllerOrbit : public IStateMachine {
      public:
        CameraControllerOrbit(std::shared_ptr<entt::registry> registry, Entity entity);
        virtual ~CameraControllerOrbit();
        void on_attach() override;
        void on_deatach() override;
        void on_update(const double& ts) override;
        void on_render() override {}
        void on_event(const SDL_Event& event) override {}
        std::string get_name() const override { return "CameraControllerOrbit"; }

      private:
        void update_vp();
        void update_vectors();
        void process_camera_rotation(const int& x_offset, const int& y_offset, bool constrain_pitch = true);
        void process_distance(const int& mz);
        void invert_pitch();

        float pitch_, yaw_, distance_, min_, max_;
        glm::vec3 up_, front_;
        Entity entity_;
        std::shared_ptr<Camera> camera_;
        std::shared_ptr<ViewProjection> vp_;

        std::shared_ptr<entt::registry> registry_;
        std::shared_ptr<InputManager> input_manager_;
    };

} // namespace ce
