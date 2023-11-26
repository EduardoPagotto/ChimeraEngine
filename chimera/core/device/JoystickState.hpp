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

    inline static double scale16(const int16_t& value) { return value >= 0 ? ((double)value) / 32767.0f : ((double)value) / 32768.0f; }
    inline uint8_t getButtonState(const uint8_t& indice) { return buttonState.contains(indice) ? buttonState[indice] : SDL_RELEASED; }
    inline uint8_t getHat(const uint8_t& indice) { return hats.contains(indice) ? hats[indice] : 0; }
    inline bool getHatDir(const uint8_t& hat, const uint8_t& dir) { return getHat(hat) & dir; }
    inline const uint8_t getId() const { return id; }
    inline const std::string getName() const { return name; }
    inline SDL_Joystick* getHandle() const { return pHandle; }

  private:
    uint8_t id = 255;
    SDL_Joystick* pHandle = nullptr;
    std::string name;
    std::map<uint8_t, uint8_t> buttonState;
    std::map<uint8_t, uint8_t> hats;
    std::map<uint8_t, int16_t> axis;
    std::map<uint8_t, int16_t> BallsX;
    std::map<uint8_t, int16_t> BallsY;
};
} // namespace Chimera
