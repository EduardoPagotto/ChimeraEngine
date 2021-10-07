#ifndef __GAME_SIMPLE_RAYCASTING__HPP
#define __GAME_SIMPLE_RAYCASTING__HPP

#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasFB.hpp"
#include "raycasting.hpp"

class Game : public Chimera::IEvents {
  public:
    Game(Chimera::CanvasFB* _pCanvas);

    virtual ~Game();
    // Inherited via IEvents
    virtual void onStart() override;
    virtual void onUpdate() override;
    virtual bool onEvent(const SDL_Event& event) override;

  private:
    Chimera::CanvasFB* pCanvas;
    // ray
    Frame* frame;
    State* state;
    World* world;
    double moveSpeed;
    double rotSpeed;
};

#endif