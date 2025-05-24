#pragma once
#include "chimera/base/GameController.hpp"
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/Mouse.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/ecs/Entity.hpp"
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
