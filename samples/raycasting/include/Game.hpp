#pragma once
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
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
    virtual std::string getName() const override { return "Game"; }

  private:
    State* state;
    World* world;
    float moveSpeed;
    float rotSpeed;
    Chimera::Canvas* canvas;
};
