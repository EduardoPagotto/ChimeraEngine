#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/device/Mouse.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(std::shared_ptr<ServiceLocator> sl);
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ServiceLocator> serviceLoc;
    std::shared_ptr<Chimera::IMouse> mouse;
};
