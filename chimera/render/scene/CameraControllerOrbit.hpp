#pragma once
#include "core/IStateMachine.hpp"
#include "core/Mouse.hpp"
#include "core/Registry.hpp"
#include "space/ICamera.hpp"

namespace ce {

class CameraControllerOrbit : public IStateMachine {
    float pitch, yaw, distance, min, max;
    glm::vec3 up, front;
    Camera* camera = nullptr;
    Entity entity;
    std::shared_ptr<IMouse> mouse;

  public:
    CameraControllerOrbit(Entity entity);
    virtual ~CameraControllerOrbit();
    void onAttach() override;
    void onDeatach() override;
    void onUpdate(const double& ts) override;
    void onRender() override {}
    bool onEvent(const SDL_Event& event) override { return true; }
    const std::string getName() override { return "Orbit"; }

  private:
    // void updateVP(IViewProjection& vp);
    void updateVP();
    void updateVectors();
    void processCameraRotation(const int& xOffset, const int& yOffset, bool constrainPitch = true);
    void processDistance(const int& _mz);
    void invertPitch();
};

} // namespace ce
