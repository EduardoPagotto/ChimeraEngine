#pragma once
#include "chimera_base/IStateMachine.hpp"
#include "chimera_base/Keyboard.hpp"
#include "chimera_base/Mouse.hpp"

class Game : public ce::IStateMachine {
  public:
    Game();
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual void onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ce::Mouse> mouse;
    std::shared_ptr<ce::Keyboard> keyboard;
};
