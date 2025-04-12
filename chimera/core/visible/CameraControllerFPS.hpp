#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/device/GameController.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "chimera/core/device/Mouse.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace ce {

class CameraControllerFPS : public IStateMachine {
  public:
    CameraControllerFPS(Entity entity);
    virtual ~CameraControllerFPS();
    void onAttach() override;
    void onDeatach() override;
    void onRender() override {}
    void onUpdate(IViewProjection& vp, const double& ts) override;
    bool onEvent(const SDL_Event& event) override {

        // FIXME: tem merda aqui!!!
        return gameControl->getEvent(event);
    }

  private:
    void updateVP(IViewProjection& vp);
    void updateVectors();
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    void processCameraMovement(glm::vec3& direction, float deltaTime);
    void invertPitch();

    float pitch, yaw, movementSpeed;
    glm::vec3 up, front, worldUp, right;
    Camera* camera = nullptr;
    Entity entity;
    std::shared_ptr<IGameController> gameControl;
    std::shared_ptr<IKeyboard> keyboard;
    std::shared_ptr<IMouse> mouse;
};
} // namespace ce