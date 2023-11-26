#pragma once
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/device/GameController.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Scene& scene);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::ViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::Registry* registry;
    Chimera::Scene* scene;
    Chimera::Solid* pCorpoRigido;
    Chimera::Label* lFPS;
    Chimera::GameController gameControl;
    int fps;
};
