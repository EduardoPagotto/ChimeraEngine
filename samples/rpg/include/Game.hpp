#pragma once
#include "core/IStateMachine.hpp"

class Game : public ce::IStateMachine {
    //  std::shared_ptr<me::IMouse> mouse;

  public:
    Game();
    virtual ~Game();
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual const std::string getName() override { return "GAME"; }
};
