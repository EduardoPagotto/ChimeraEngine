#ifndef __CHIMERA_IGAME_CLIENT_EVENTS__HPP
#define __CHIMERA_IGAME_CLIENT_EVENTS__HPP

#include "JoystickManager.h"

namespace Chimera {

class IGameClientEvents {
  public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void render() = 0;
    virtual void keyCapture(SDL_Keycode tecla) = 0;
    virtual void mouseButtonDownCapture(SDL_MouseButtonEvent mb) = 0;
    virtual void mouseButtonUpCapture(SDL_MouseButtonEvent mb) = 0;
    virtual void mouseMotionCapture(SDL_MouseMotionEvent mm) = 0;
    virtual void joystickCapture(JoystickManager& joy) = 0;
    virtual void joystickStatus(JoystickManager& joy) = 0;
    virtual void userEvent(const SDL_Event& _event) = 0;
    virtual void newFPS(const unsigned int& fps) = 0;
    virtual void windowEvent(const SDL_WindowEvent& _event) = 0;
    virtual bool paused() = 0;
};

} // namespace Chimera
#endif
