#pragma once
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/device/interfaces.hpp"
#include "raycasting.hpp"

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
    State* state;
    World* world;
    float moveSpeed;
    float rotSpeed;
    std::shared_ptr<ce::ICanva> canvas;
    std::shared_ptr<ce::IKeyboard> keyboard;
};
