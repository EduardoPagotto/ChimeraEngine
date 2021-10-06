#ifndef __CHIMERA_CORE_IEVENTS__HPP
#define __CHIMERA_CORE_IEVENTS__HPP

#include "JoystickState.hpp"
#include "MouseDevice.hpp"
#include <SDL2/SDL.h>

namespace Chimera::Core {

class IEvents {
  public:
    virtual void update() = 0;
    virtual void keboardEvent(SDL_Event* pEventSDL) = 0;
    virtual void mouseEvent(SDL_Event* pEventSDL) = 0;
    virtual void joystickEvent(SDL_Event* pEventSDL) = 0;

    virtual void userEvent(const SDL_Event& _event) = 0;
    virtual void windowEvent(const SDL_WindowEvent& _event) = 0;
};

} // namespace Chimera::Core
#endif
