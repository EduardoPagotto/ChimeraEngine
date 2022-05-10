#pragma once
#include "chimera/core/Engine.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/scene/Scene.hpp"

struct Controles {
    float yaw;
    float pitch;
    float roll;
    float throttle;
    int hat;
};

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Scene* scene, Chimera::Engine* engine);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override { return "Game"; }

  private:
    Chimera::Scene* scene;
    Chimera::Solid* pCorpoRigido;
    Chimera::Label* lFPS;
    Controles crt;
    int fps;
};
