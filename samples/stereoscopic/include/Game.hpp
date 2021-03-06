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
    virtual void keyCapture(SDL_Keycode tecla);
    virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb);
    virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb);
    virtual void mouseMotionCapture(SDL_MouseMotionEvent mm);
    virtual void joystickCapture(Chimera::JoystickManager& joy);
    virtual void joystickStatus(Chimera::JoystickManager& joy);
    virtual void newFPS(const unsigned int& fps);
    virtual void userEvent(const SDL_Event& _event) override;
    virtual void windowEvent(const SDL_WindowEvent& _event) override;
    virtual bool paused() override;

  private:
    bool isPaused;
    int botaoIndex;
    int estadoBotao;

    Chimera::VisitorRender renderV;
    Chimera::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
};

#endif