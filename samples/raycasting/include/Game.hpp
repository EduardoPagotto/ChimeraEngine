#ifndef __GAME_SIMPLE_RAYCASTING__HPP
#define __GAME_SIMPLE_RAYCASTING__HPP

#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "raycasting.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::CanvasFB* _pCanvas);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void update();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::Core::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::Core::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;

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