#ifndef __CHIMERA_JOYSTICK_MANAGER__HPP
#define __CHIMERA_JOYSTICK_MANAGER__HPP

#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "chimera/core/JoystickState.hpp"

namespace Chimera {

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
 * Enumerador dos PAD's do Joystic XBOX360
 */
enum class JOY_PAD_COD { UP = 0b00000001, RIGHT = 0b00000010, DOWN = 0b00000100, LEFT = 0b00001000 };

/**
 * Classe gerenciadora de todos os Joysticks identificados
 */
class JoystickManager {
  public:
    JoystickManager(void);
    virtual ~JoystickManager(void);

    void Initialize(void);
    void ReleaseJoysticks(void);
    void FindJoysticks(void);
    void GetStatusManager(void);
    void DebugDadosJoystick();
    JoystickState* getJoystickState(const uint8_t& joystick_id);
    JoystickState* setAxisMotion(SDL_Event* event);
    JoystickState* setButtonStateDown(SDL_Event* event);
    JoystickState* setButtonStateUp(SDL_Event* event);
    JoystickState* setHatMotion(SDL_Event* event);
    JoystickState* setBallMotion(SDL_Event* event);

  private:
    // JoystickState* getJoystickState(const uint8_t& joystick_id);
    std::map<uint8_t, JoystickState> Joysticks;
    bool Initialized;
};
} // namespace Chimera
#endif
