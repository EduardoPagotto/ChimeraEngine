#include "cevk_infra/Keyboard.hpp"

namespace ce {

    void Keyboard::setDown(const SDL_KeyboardEvent& event) noexcept {
        mapKey[event.key] = true;
        mod = event.mod;
    }

    void Keyboard::setUp(const SDL_KeyboardEvent& event) noexcept {
        mapKey[event.key] = false;
        mod = event.mod;
    }

    bool Keyboard::isPressed(const SDL_Keycode& key) noexcept {

        if (mapKey.contains(key)) {
            return (mapKey[key]);
        }

        return false;
    }

    bool Keyboard::getEvent(const SDL_Event& event) noexcept {

        switch (event.type) {
            case SDL_EVENT_KEY_DOWN:
                Keyboard::setDown(event.key);
                break;
            case SDL_EVENT_KEY_UP:
                Keyboard::setUp(event.key);
                break;
            default:
                break;
        }

        return false;
    }
} // namespace ce
