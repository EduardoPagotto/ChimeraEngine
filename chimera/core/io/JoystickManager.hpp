#ifndef __CHIMERA_CORE_JOYSTICK_MANAGER__HPP
#define __CHIMERA_CORE_JOYSTICK_MANAGER__HPP

#include "JoystickState.hpp"
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {

class JoystickManager {
  public:
    static void init(void);
    static void release(void);
    static void find(void);
    static void getStatusManager(void);
    static void debug();

    static void setAxisMotion(SDL_JoyAxisEvent* jaxis);
    static void setButtonState(SDL_JoyButtonEvent* jbutton);
    static void setHatMotion(SDL_JoyHatEvent* jhat);
    static void setBallMotion(SDL_JoyBallEvent* jball);

    static JoystickState* select(const SDL_JoystickID& joystick_id);

  private:
    static std::map<SDL_JoystickID, JoystickState> Joysticks;
    static bool Initialized;
};
} // namespace Chimera
#endif
