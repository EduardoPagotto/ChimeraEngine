#pragma once
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "chimera/core/device/Keyboard.hpp"
#include "raycasting.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine);

    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(Chimera::ViewProjection& vp, const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    State* state;
    World* world;
    float moveSpeed;
    float rotSpeed;
    Chimera::ICanva* canvas;
    Chimera::Keyboard* keyboard;
};
