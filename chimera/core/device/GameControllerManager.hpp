#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {

// struct GamePad {
//     SDL_GameController* handle;
//     SDL_JoystickID id;

//     std::map<uint8_t, uint8_t> buttonState;
//     std::map<uint8_t, uint8_t> hats;
//     std::map<uint8_t, int16_t> axis;
// };

class GameControllerManager {
  public:
    static void init(void);
    static void release(void);
    static void add(const SDL_ControllerDeviceEvent& device);
    static void remover(const SDL_ControllerDeviceEvent& device);
    static void getStatusManager(void);
    static void debug();

    static bool getEvent(const SDL_Event& event);

    static void getGamePad();

    // static void setAxisMotion(SDL_JoyAxisEvent* jaxis);
    // static void setButtonState(SDL_JoyButtonEvent* jbutton);
    // static void setHatMotion(SDL_JoyHatEvent* jhat);
    // static void setBallMotion(SDL_JoyBallEvent* jball);

    static SDL_GameController* select(const SDL_JoystickID& joystick_id);

  private:
    inline static std::map<SDL_JoystickID, SDL_GameController*> pads;
};
} // namespace Chimera
