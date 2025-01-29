#pragma once
#include "core/GameController.hpp"
#include "core/IStateMachine.hpp"
#include "core/Keyboard.hpp"
#include "core/Mouse.hpp"
#include "core/Registry.hpp"
#include "space/ICamera.hpp"

namespace ce {

class CameraControllerFPS : public IStateMachine {
    float pitch, yaw, movementSpeed;
    glm::vec3 up, front, worldUp, right;
    Camera* camera = nullptr;
    Entity entity;
    std::shared_ptr<IGameController> gameControl;
    std::shared_ptr<IKeyboard> keyboard;
    std::shared_ptr<IMouse> mouse;

  public:
    CameraControllerFPS(Entity entity);
    virtual ~CameraControllerFPS();
    void onAttach() override;
    void onDeatach() override;
    void onRender() override {}
    void onUpdate(const double& ts) override;
    const std::string getName() override { return "FPS"; }
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
};
} // namespace ce
