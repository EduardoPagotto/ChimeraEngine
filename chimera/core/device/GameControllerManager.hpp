#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {
class GameControllerManager {
  public:
    static void init(void);
    static void release(void);
    static bool getEvent(const SDL_Event& event);
    static SDL_GameController* get(const SDL_JoystickID& pad_id) { return pads.contains(pad_id) ? pads[pad_id] : nullptr; }

  private:
    static void added();
    static void removed(const SDL_ControllerDeviceEvent& device);
    inline static std::map<SDL_JoystickID, SDL_GameController*> pads;
};
} // namespace Chimera
