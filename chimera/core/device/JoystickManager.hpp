#pragma once

#include "JoystickState.hpp"

namespace Chimera {

class JoystickManager {
  public:
    static void init(void);
    static void release(void);
    static void find(void);
    static void getStatusManager(void);
    static void debug();
    static void debugJoy(const JoystickState& j);

    static void setAxisMotion(const SDL_JoyAxisEvent& jaxis);
    static void setButtonState(const SDL_JoyButtonEvent& jbutton);
    static void setHatMotion(const SDL_JoyHatEvent& jhat);
    static void setBallMotion(const SDL_JoyBallEvent& jball);
    static bool getEvent(const SDL_Event& event);

    static JoystickState* select(const SDL_JoystickID& joystick_id) {
        return Joysticks.contains(joystick_id) ? &Joysticks[joystick_id] : nullptr;
    }

  private:
    inline static std::map<SDL_JoystickID, JoystickState> Joysticks;
};
} // namespace Chimera
