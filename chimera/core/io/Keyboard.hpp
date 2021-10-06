#ifndef __CHIMERA_CORE_KEYBOARD_CONTROL__HPP
#define __CHIMERA_CORE_KEYBOARD_CONTROL__HPP

#include <SDL2/SDL.h>
#include <map>

namespace Chimera::Core {

class Keyboard {
  public:
    static void setDown(SDL_KeyboardEvent event);
    static void setUp(SDL_KeyboardEvent event);
    static bool isPressed(SDL_Keycode key);
    static bool isModPressed(SDL_Keymod keyMod);

  private:
    static std::map<SDL_Keycode, uint8_t> mapKey;
    static uint16_t mod;
};
} // namespace Chimera::Core
#endif