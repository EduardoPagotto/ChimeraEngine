#ifndef __CHIMERA_CORE_JOYSTICK_STATE__HPP
#define __CHIMERA_CORE_JOYSTICK_STATE__HPP

#include <map>
#include <string>

#include <SDL2/SDL.h>

namespace Chimera::Core {

/**
 * Enumerador dos codigos dos Axis do Joystico do XBOX360
 */
enum class JOY_AXIX_COD {
    LEFT_X = 0,
    LEFT_Y = 1,
    LEFT_TRIGGER = 2,
    RIGHT_X = 3,
    RIGHT_Y = 4,
    RIGHT_TRIGGER = 5,
};

/**
 * Enumerador dos codigos dos botoes do Joystic XBOX360
 */
enum class JOY_BUTTON_COD {
    A,
    B,
    X,
    Y,
    LEFT_BUMPER,
    RIGHT_BUMPER,
    BACK,
    START,
    LEFT_STICK_CLICK,
    RIGHT_STICK_CLICK,
};

/**
 * Classe de controle do Joystick
 */
class JoystickState {
  public:
    JoystickState();

    friend class JoystickManager;

    inline static double scale16(const int16_t& value) { return value >= 0 ? ((double)value) / 32767.0f : ((double)value) / 32768.0f; }

    int16_t getAxis(const uint8_t& index, const int16_t& deadzone = 0, const int16_t& deadzone_at_ends = 0);
    uint8_t getButtonState(const uint8_t& indice);
    uint8_t getHat(const uint8_t& indice);
    inline bool getHatDir(const uint8_t& hat, const uint8_t& dir) { return getHat(hat) & dir; }

    void debug(void);

    inline uint8_t getId() const { return id; }
    inline std::string getName() const { return name; }

  private:
    uint8_t id;
    std::string name;
    SDL_Joystick* pHandle;
    std::map<uint8_t, uint8_t> buttonState;
    std::map<uint8_t, uint8_t> hats;
    std::map<uint8_t, int16_t> axis;
    std::map<uint8_t, int16_t> BallsX;
    std::map<uint8_t, int16_t> BallsY;
};
} // namespace Chimera::Core

#endif
