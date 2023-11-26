#pragma once

#include "JoystickState.hpp"

namespace Chimera {

class JoystickManager {
  public:
    static void init(void);
    static void release(void);
    static void find(void);
    static bool getEvent(const SDL_Event& event);
    static void remover(const SDL_ControllerDeviceEvent& device);
    static SDL_Joystick* select(const SDL_JoystickID& joystick_id) { return joys.contains(joystick_id) ? joys[joystick_id] : nullptr; }

    inline static double scale16(const int16_t& value) { return value >= 0 ? ((double)value) / 32767.0f : ((double)value) / 32768.0f; }

  private:
    inline static std::map<SDL_JoystickID, SDL_Joystick*> joys;
};
} // namespace Chimera
