#pragma once
#include "chimera/base/GamePad.hpp"
#include "chimera/base/ICamera.hpp"
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/Keyboard.hpp"
#include "chimera/base/Mouse.hpp"
#include "chimera/base/ViewProjection.hpp"
#include "chimera/ecs/Entity.hpp"

namespace ce {

    class CameraControllerFPS : public IStateMachine {

      private:
        float pitch, yaw, movementSpeed;
        glm::vec3 up, front, worldUp, right;
        Entity entity;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<ViewProjection> vp;
        std::shared_ptr<GamePad> gameControl;
        std::shared_ptr<Keyboard> keyboard;
        std::shared_ptr<Mouse> mouse;

      public:
        CameraControllerFPS(Entity entity);
        virtual ~CameraControllerFPS();
        void onAttach() override;
        void onDeatach() override;
        void onRender() override {}
        void onUpdate(const double& ts) override;
        bool onEvent(const SDL_Event& event) override {

            // FIXME: tem merda aqui!!!
            return gameControl->getEvent(event);
        }

      private:
        void updateVP();
        void updateVectors();
        void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
        void processCameraMovement(glm::vec3& direction, float deltaTime);
        void invertPitch();
        void processCameraFOV(const float& offset);
    };
} // namespace ce
