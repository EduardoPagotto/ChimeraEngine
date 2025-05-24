#pragma once
#include "chimera/base/ICamera.hpp"
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/Mouse.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {

    class CameraControllerOrbit : public IStateMachine {
      public:
        CameraControllerOrbit(Entity entity);
        virtual ~CameraControllerOrbit();
        void onAttach() override;
        void onDeatach() override;
        void onUpdate(IViewProjection& vp, const double& ts) override;
        void onRender() override {}
        bool onEvent(const SDL_Event& event) override { return true; }

      private:
        void updateVP(IViewProjection& vp);
        void updateVectors();
        void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
        void processDistance(const int& _mz);
        void invertPitch();

        float pitch, yaw, distance, min, max;
        glm::vec3 up, front;
        Camera* camera = nullptr;
        Entity entity;
        std::shared_ptr<IMouse> mouse;
    };

} // namespace ce
