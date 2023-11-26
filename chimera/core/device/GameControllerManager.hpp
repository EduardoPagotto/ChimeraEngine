#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {
class GameControllerManager {
  public:
    static void init(void);
    static void release(void);
    static void find();
    static void remover(const SDL_ControllerDeviceEvent& device);
    static bool getEvent(const SDL_Event& event);
    static SDL_GameController* select(const SDL_JoystickID& pad_id) { return pads.contains(pad_id) ? pads[pad_id] : nullptr; }

  private:
    inline static std::map<SDL_JoystickID, SDL_GameController*> pads;
};
} // namespace Chimera
