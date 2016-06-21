#ifndef __IGAME_CLIENT__H_
#define __IGAME_CLIENT__H_

# include "JoystickManager.h"

namespace Chimera
{

class IGameClientEvents
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void render() = 0;
    virtual void keyCapture ( SDL_Keycode tecla ) = 0;
    virtual void mouseButtonDownCapture ( SDL_MouseButtonEvent mb ) = 0;
    virtual void mouseButtonUpCapture ( SDL_MouseButtonEvent mb ) = 0;
    virtual void mouseMotionCapture ( SDL_MouseMotionEvent mm ) = 0;
    virtual void joystickCapture ( Device::JoystickManager &joy ) = 0;
    virtual void joystickStatus ( Device::JoystickManager &joy ) = 0;
    virtual void userEvent ( const SDL_Event &_event ) = 0;
    virtual void newFPS ( const unsigned int &fps ) = 0;
    virtual void beginProcGame() = 0;
    virtual void endProcGame() = 0;
};


}

#endif