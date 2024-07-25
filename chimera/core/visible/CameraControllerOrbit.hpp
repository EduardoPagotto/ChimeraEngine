#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/device/Mouse.hpp"
#include "chimera/core/visible/ICamera.hpp"

namespace Chimera {

class CameraControllerOrbit : public IStateMachine {
  public:
    CameraControllerOrbit(std::shared_ptr<ServiceLocator> serviceLocator, Entity entity);
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

} // namespace Chimera
