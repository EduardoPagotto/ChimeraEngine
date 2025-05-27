#pragma once
#include "chimera/base/ICamera.hpp"
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/Mouse.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {

    class CameraControllerOrbit : public IStateMachine {

      private:
        float pitch, yaw, distance, min, max;
        glm::vec3 up, front;
        std::shared_ptr<Camera> camera;
        Entity entity;
        std::shared_ptr<Mouse> mouse;
        std::shared_ptr<ViewProjection> vp;

      public:
        CameraControllerOrbit(Entity entity);
        virtual ~CameraControllerOrbit();
        void onAttach() override;
        void onDeatach() override;
        void onUpdate(const double& ts) override;
        void onRender() override {}
        bool onEvent(const SDL_Event& event) override { return true; }

      private:
        void updateVP();
        void updateVectors();
        void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
        void processDistance(const int& _mz);
        void invertPitch();
    };

} // namespace ce
