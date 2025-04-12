#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/device/GameController.hpp"
#include "chimera/core/device/Mouse.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public ce::IStateMachine {
  public:
    Game(ce::Scene* scene);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(ce::IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ce::IMouse> mouse;
    std::shared_ptr<ce::IGameController> gameControl;

    ce::Scene* scene;
    ce::Solid* pCorpoRigido;
    ce::Label* lFPS;
    int fps;
};
