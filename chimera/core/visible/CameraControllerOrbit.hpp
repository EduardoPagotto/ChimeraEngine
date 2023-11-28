#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/device/MouseDevice.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraControllerOrbit : public IStateMachine {
  public:
    CameraControllerOrbit(Entity entity) : IStateMachine("Orbit"), entity(entity) {}
    void onAttach() override;
    void onDeatach() override;
    void onUpdate(ViewProjection& vp, const double& ts) override;
    void onRender() override {}
    bool onEvent(const SDL_Event& event) override { return true; }

  private:
    void updateVP(ViewProjection& vp);
    void updateVectors();
    void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
    void processDistance(const int& _mz);
    void invertPitch();

    float pitch, yaw, distance, min, max;
    glm::vec3 up, front;
    Camera* camera = nullptr;
    Entity entity;
    MouseDevice* mouse;
};

} // namespace Chimera
