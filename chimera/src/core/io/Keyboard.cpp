#include "chimera/core/io/Keyboard.hpp"

namespace Chimera {

std::map<SDL_Keycode, uint8_t> Keyboard::mapKey;
uint16_t Keyboard::mod;

void Keyboard::setDown(SDL_KeyboardEvent event) {
    mapKey[event.keysym.sym] = SDL_PRESSED;
    mod = event.keysym.mod;
}

void Keyboard::setUp(SDL_KeyboardEvent event) {
    mapKey[event.keysym.sym] = SDL_RELEASED;
    mod = event.keysym.mod;
}

bool Keyboard::isModPressed(SDL_Keymod keyMod) { return (keyMod & mod); }

bool Keyboard::isPressed(SDL_Keycode key) {

    auto it = mapKey.find(key);
    if (it != mapKey.end())
        return mapKey[key] == SDL_PRESSED;

    return false;
}

} // namespace Chimera