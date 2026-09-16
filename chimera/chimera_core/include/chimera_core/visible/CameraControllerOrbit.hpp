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
        void onAttach() override;
        void onDeatach() override;
        void onUpdate(const double& ts) override;
        void onRender() override {}
        void onEvent(const SDL_Event& event) override {}
        std::string getName() const override { return "CameraControllerOrbit"; }

      private:
        void updateVP();
        void updateVectors();
        void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
        void processDistance(const int& _mz);
        void invertPitch();

        float pitch, yaw, distance, min, max;
        glm::vec3 up, front;
        Entity entity;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<ViewProjection> vp;

        std::shared_ptr<entt::registry> registry;
        std::shared_ptr<InputManager> inputManager;
    };

} // namespace ce
