#pragma once
#include "chimera/core/Engine.hpp"
#include "chimera/core/IStateMachine.hpp"
#include "raycasting.hpp"

class Game : public Chimera::IStateMachine {
  public:
    Game(Chimera::Engine* engine) : engine(engine){};

    virtual ~Game();
    // Inherited via IEvents
    virtual void onAttach() override;
    virtual void onDeatach() override;
    virtual void onRender() override;
    virtual void onUpdate(const double& ts) override;
    virtual bool onEvent(const SDL_Event& event) override;
    virtual std::string getName() const override { return "Game"; }

  private:
    // ray
    Frame* frame;
    State* state;
    World* world;
    double moveSpeed;
    double rotSpeed;
    Chimera::Engine* engine;
};
