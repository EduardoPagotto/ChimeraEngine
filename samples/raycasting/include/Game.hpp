#ifndef __GAME_SIMPLE_RAYCASTING__HPP
#define __GAME_SIMPLE_RAYCASTING__HPP

#include "chimera/core/CanvasFB.hpp"
#include "chimera/core/io/IEvents.hpp"
#include "raycasting.hpp"

class Game : public Chimera::IO::IEvents {
  public:
    Game(Chimera::CanvasFB* _pCanvas);

    virtual ~Game();
    // Inherited via IEvents
    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::IO::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::IO::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void newFPS(const unsigned int& fps);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    Chimera::CanvasFB* pCanvas;
    bool isPaused;

    // ray
    Frame* frame;
    State* state;
    World* world;
    double moveSpeed;
    double rotSpeed;
};

#endif