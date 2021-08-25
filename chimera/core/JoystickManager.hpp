#ifndef __CHIMERA_JOYSTICK_MANAGER__HPP
#define __CHIMERA_JOYSTICK_MANAGER__HPP

#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "chimera/core/JoystickState.hpp"

namespace Chimera {

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
