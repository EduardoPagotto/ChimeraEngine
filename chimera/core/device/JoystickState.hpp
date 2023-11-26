#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>

namespace Chimera {

/**
 * Classe de controle do Joystick
 */
class JoystickState {
    friend class JoystickManager;

  public:
    JoystickState() : id(255), pHandle(nullptr), name("none") {}

    inline int16_t getAxis(const uint8_t& index, const int16_t& deadzone = 0, const int16_t& deadzone_at_ends = 0) {
        int16_t value = axis.contains(index) ? axis[index] : 0;
        return fabs(value) > deadzone ? value : 0;
    }

  private:
    SDL_JoystickID id = -1;
    SDL_Joystick* pHandle = nullptr;
    std::string name;
    std::map<uint8_t, uint8_t> buttonState;
    std::map<uint8_t, uint8_t> hats;
    std::map<uint8_t, int16_t> axis;
    std::map<uint8_t, int16_t> BallsX;
    std::map<uint8_t, int16_t> BallsY;
};
} // namespace Chimera
