#ifndef __CHIMERA_JOYSTICK_STATE__HPP
#define __CHIMERA_JOYSTICK_STATE__HPP

#include <map>
#include <string>

#include <SDL2/SDL.h>

namespace Chimera {

/**
 * Classe de controle do Joystick
 */
class JoystickState {
  public:
    JoystickState();

    void TrackEvent(SDL_Event* event);

    inline static double scale16(const int16_t& value) { return value >= 0 ? ((double)value) / 32767.0f : ((double)value) / 32768.0f; }

    int16_t getAxis(const uint8_t& index, const int16_t& deadzone = 0, const int16_t& deadzone_at_ends = 0);

    bool getButton(const uint8_t& indice);
    uint8_t getButtonState(const uint8_t& indice);
    uint8_t getHat(const uint8_t& indice);
    inline bool getHatDir(const uint8_t& hat, const uint8_t& dir) { return getHat(hat) & dir; }

    void debug(void);

    // verificar
    uint8_t id;
    std::string name;
    SDL_Joystick* pJoystick;

  private:
    std::map<uint8_t, bool> button;
    std::map<uint8_t, uint8_t> buttonState;
    std::map<uint8_t, uint8_t> hats;
    std::map<uint8_t, int16_t> axis;
    std::map<uint8_t, int16_t> BallsX;
    std::map<uint8_t, int16_t> BallsY;
};
} // namespace Chimera

#endif
