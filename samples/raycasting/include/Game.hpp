#ifndef __GAME_SIMPLE_RAYCASTING__HPP
#define __GAME_SIMPLE_RAYCASTING__HPP

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
    virtual void onUpdate(const uint32_t& count) override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    // ray
    Frame* frame;
    State* state;
    World* world;
    double moveSpeed;
    double rotSpeed;
    Chimera::Engine* engine;
};

#endif