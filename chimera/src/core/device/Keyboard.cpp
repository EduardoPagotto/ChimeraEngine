#include "chimera/core/device/Keyboard.hpp"

namespace Chimera {

void Keyboard::setDown(const SDL_KeyboardEvent& event) {
    mapKey[event.keysym.sym] = SDL_PRESSED;
    mod = event.keysym.mod;
}

void Keyboard::setUp(const SDL_KeyboardEvent& event) {
    mapKey[event.keysym.sym] = SDL_RELEASED;
    mod = event.keysym.mod;
}

bool Keyboard::isModPressed(const SDL_Keymod& keyMod) { return (keyMod & mod); }

bool Keyboard::isPressed(const SDL_Keycode& key) {

    if (auto it = mapKey.find(key); it != mapKey.end())
        return mapKey[key] == SDL_PRESSED;

    return false;
}

bool Keyboard::getEvent(const SDL_Event& event) {

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

} // namespace Chimera