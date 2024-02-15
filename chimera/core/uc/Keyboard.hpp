#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {
class Keyboard {
  private:
    std::map<SDL_Keycode, uint8_t> mapKey;
    uint16_t mod;

  public:
    Keyboard() = default;
    virtual ~Keyboard() = default;

    inline void setDown(const SDL_KeyboardEvent& event) {
        mapKey[event.keysym.sym] = SDL_PRESSED;
        mod = event.keysym.mod;
    }

    inline void setUp(const SDL_KeyboardEvent& event) {
        mapKey[event.keysym.sym] = SDL_RELEASED;
        mod = event.keysym.mod;
    }

    inline const bool isPressed(const SDL_Keycode& key) {

        if (mapKey.contains(key))
            return (mapKey[key] == SDL_PRESSED);

        return false;
    }

    inline const bool isModPressed(const SDL_Keymod& keyMod) const { return (keyMod & mod); }

    inline const bool getEvent(const SDL_Event& event) {

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
};
} // namespace Chimera
