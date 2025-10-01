#pragma once
#include "chimera/base/IStateMachine.hpp"
#include "chimera/base/Keyboard.hpp"
#include "raycasting.hpp"

class Game : public ce::IStateMachine {
    State* state;
    World* world;
    float moveSpeed;
    float rotSpeed;
    std::shared_ptr<ce::ICanva> canvas;
    std::shared_ptr<ce::Keyboard> keyboard;

  public:
    Game();
    virtual ~Game();

    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
};
