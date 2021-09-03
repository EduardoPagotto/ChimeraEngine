#ifndef __CHIMERA_IGAME_CLIENT_EVENTS__HPP
#define __CHIMERA_IGAME_CLIENT_EVENTS__HPP

#include "chimera/core/JoystickState.hpp"
#include "chimera/core/MouseDevice.hpp"
#include <SDL2/SDL.h>

namespace Chimera {

class IEvents {
  public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void render() = 0;
    virtual void keboardEvent(SDL_Keycode tecla) = 0;
    virtual void mouseEvent(MouseDevice* pMouse, SDL_Event* pEventSDL) = 0;
    virtual void joystickEvent(JoystickState* pJoy, SDL_Event* pEventSDL) = 0;
    virtual void userEvent(const SDL_Event& _event) = 0;
    virtual void newFPS(const unsigned int& fps) = 0;
    virtual void windowEvent(const SDL_WindowEvent& _event) = 0;
    virtual bool paused() = 0;
};

} // namespace Chimera
#endif
