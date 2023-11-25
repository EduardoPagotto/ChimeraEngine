#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace Chimera {

class Keyboard {
  public:
    static void setDown(const SDL_KeyboardEvent& event);
    static void setUp(const SDL_KeyboardEvent& event);
    static bool isPressed(const SDL_Keycode& key);
    static bool isModPressed(const SDL_Keymod& keyMod);
    static bool getEvent(const SDL_Event& event);

  private:
    inline static std::map<SDL_Keycode, uint8_t> mapKey;
    inline static uint16_t mod;
};
} // namespace Chimera
