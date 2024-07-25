#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/Registry.hpp"
#include "chimera/core/bullet/Solid.hpp"
#include "chimera/core/device/GameController.hpp"
#include "chimera/core/device/Mouse.hpp"
#include "chimera/render/2d/Label.hpp"
#include "chimera/render/scene/Scene.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(std::shared_ptr<ServiceLocator> sl, Chimera::Scene* scene);
    virtual ~Game();

    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ServiceLocator> serviceLoc;
    std::shared_ptr<Chimera::IMouse> mouse;
    std::shared_ptr<Chimera::IGameController> gameControl;
    std::shared_ptr<Chimera::Registry> registry;

    Chimera::Scene* scene;
    Chimera::Solid* pCorpoRigido;
    Chimera::Label* lFPS;
    int fps;
};
