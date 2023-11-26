#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {

class JoystickManager {
  public:
    static void init(void);
    static void release(void);
    static bool getEvent(const SDL_Event& event);
    static SDL_Joystick* get(const SDL_JoystickID& joystick_id) { return joys.contains(joystick_id) ? joys[joystick_id] : nullptr; }

  private:
    static void added(void);
    static void removed(const SDL_ControllerDeviceEvent& device);
    inline static std::map<SDL_JoystickID, SDL_Joystick*> joys;
};
} // namespace Chimera
