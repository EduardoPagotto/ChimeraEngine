#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraControllerFPS : public IStateMachine {
  public:
    CameraControllerFPS(Entity entity) : entity(entity) {}
    void onAttach() override;
    void onDeatach() override;
    void onRender() override {}
    void onUpdate(ViewProjection& vp, const double& ts) override;
    bool onEvent(const SDL_Event& event) override { return true; }
    std::string getName() const override { return "FPS"; }

  private:
    void updateVP(ViewProjection& vp);
    void updateVectors();
    void processCameraRotation(double xOffset, double yOffset, bool constrainPitch);
    void processCameraMovement(glm::vec3& direction, float deltaTime);
    void invertPitch();

    float pitch, yaw, movementSpeed;
    glm::vec3 up, front, worldUp, right;
    Camera* camera = nullptr;
    Entity entity;
};
} // namespace Chimera