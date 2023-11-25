#pragma once
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/scene/Scene.hpp"

struct Controles {
    Controles() = default;
    float yaw = 0.0, pitch = 0.0, roll = 0.0, throttle = 0.0;
    int hat = 0;
};

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Scene& scene);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::ViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override { return "Game"; }

  private:
    Chimera::Registry* registry;
    Chimera::Scene* scene;
    Chimera::Solid* pCorpoRigido;
    Chimera::Label* lFPS;
    Controles crt;
    int fps;
};
