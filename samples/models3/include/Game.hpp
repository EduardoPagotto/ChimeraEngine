#pragma once
#include "chimera_base/GamePad.hpp"
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/Mouse.hpp"
#include "chimera_core/bullet/Solid.hpp"
#include "chimera_ecs/Entity.hpp"
#include "chimera_render/2d/Label.hpp"
#include "chimera_render/scene/Scene.hpp"

class Game : public ce::IStateMachine {
  public:
    Game(ce::Scene* scene);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual void onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ce::Mouse> mouse;
    std::shared_ptr<ce::GamePad> gameControl;

    ce::Scene* scene;
    ce::Solid* pCorpoRigido;
    ce::Label* lFPS;
    int fps;
};
