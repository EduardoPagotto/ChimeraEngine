#ifndef __CHIMERA_CORE_IEVENTS__HPP
#define __CHIMERA_CORE_IEVENTS__HPP

#include "JoystickState.hpp"
#include "MouseDevice.hpp"
#include <SDL2/SDL.h>

namespace Chimera::IO {

class IEvents {
  public:
    virtual void update() = 0;
    virtual void keboardEvent(SDL_Keycode tecla) = 0;
    virtual void mouseEvent(MouseDevice* pMouse, SDL_Event* pEventSDL) = 0;
    virtual void joystickEvent(JoystickState* pJoy, SDL_Event* pEventSDL) = 0;
    virtual void userEvent(const SDL_Event& _event) = 0;
    virtual void windowEvent(const SDL_WindowEvent& _event) = 0;
};

} // namespace Chimera::IO
#endif
