#ifndef __GAME_SIMPLE_RAYCASTING__HPP
#define __GAME_SIMPLE_RAYCASTING__HPP

#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasFB.hpp"
#include "raycasting.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::Core::CanvasFB* _pCanvas);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void update();
    virtual void keboardEvent(SDL_Event* pEventSDL);
    virtual void mouseEvent(SDL_Event* pEventSDL);
    virtual void joystickEvent(SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

  private:
    Chimera::Core::CanvasFB* pCanvas;
    // ray
    Frame* frame;
    State* state;
    World* world;
    double moveSpeed;
    double rotSpeed;
};

#endif