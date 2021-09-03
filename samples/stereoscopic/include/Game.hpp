#ifndef __GAME_EVENTS_TEST__HPP
#define __GAME_EVENTS_TEST__HPP

#include "chimera/core/CanvasGL.hpp"
#include "chimera/core/IGameClientEvents.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/VisitorRender.hpp"

class Game : public Chimera::IGameClientEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Node* _pRoot);
    virtual ~Game();
    // Inherited via IGameClientEvents
    virtual void start();
    virtual void stop();
    virtual void render();
    virtual void keboardEvent(SDL_Keycode tecla);
    virtual void mouseEvent(Chimera::MouseDevice* pMouse, SDL_Event* pEventSDL);
    virtual void joystickEvent(Chimera::JoystickState* pJoy, SDL_Event* pEventSDL);
    virtual void newFPS(const unsigned int& fps);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    bool isPaused;

    Chimera::VisitorRender renderV;
    Chimera::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
};

#endif