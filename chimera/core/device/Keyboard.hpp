#pragma once

#include <SDL2/SDL.h>
#include <map>

namespace Chimera {

class Keyboard {
  public:
    static void setDown(const SDL_KeyboardEvent& event) {
        mapKey[event.keysym.sym] = SDL_PRESSED;
        mod = event.keysym.mod;
    }

    static void setUp(const SDL_KeyboardEvent& event) {
        mapKey[event.keysym.sym] = SDL_RELEASED;
        mod = event.keysym.mod;
    }

    static bool isPressed(const SDL_Keycode& key) {
        if (auto it = mapKey.find(key); it != mapKey.end())
            return mapKey[key] == SDL_PRESSED;

        return false;
    }

    static bool isModPressed(const SDL_Keymod& keyMod) { return (keyMod & mod); }

    static bool getEvent(const SDL_Event& event) {

        switch (event.type) {
            case SDL_KEYDOWN:
                Keyboard::setDown(event.key);
                break;
            case SDL_KEYUP:
                Keyboard::setUp(event.key);
                break;
        }

        return false;
    }

  private:
    inline static std::map<SDL_Keycode, uint8_t> mapKey;
    inline static uint16_t mod;
};
} // namespace Chimera
