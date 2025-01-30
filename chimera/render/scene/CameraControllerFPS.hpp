#pragma once
#include "chimera/core/GameController.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Keyboard.hpp"
#include "chimera/core/Mouse.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/space/ICamera.hpp"

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
