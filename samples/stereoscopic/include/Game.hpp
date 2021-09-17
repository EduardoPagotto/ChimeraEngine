#ifndef __GAME_EVENTS_TEST__HPP
#define __GAME_EVENTS_TEST__HPP

#include "chimera/core/io/IEvents.hpp"
#include "chimera/core/windows/CanvasGL.hpp"
#include "chimera/node/Node.hpp"
#include "chimera/node/VisitorRender.hpp"

class Game : public Chimera::Core::IEvents {
  public:
    Game(Chimera::CanvasGL* _pCanvas, Chimera::Node* _pRoot);
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
    Chimera::VisitorRender renderV;
    Chimera::CanvasGL* pCanvas;
    Chimera::Node* pRoot;
};

#endif