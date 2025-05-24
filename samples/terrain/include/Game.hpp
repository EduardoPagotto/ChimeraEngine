#pragma once
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/Keyboard.hpp"
#include "chimera/base/Mouse.hpp"

class Game : public ce::IStateMachine {
  public:
    Game();
    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(ce::IViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    std::shared_ptr<ce::IMouse> mouse;
    std::shared_ptr<ce::IKeyboard> keyboard;
};
