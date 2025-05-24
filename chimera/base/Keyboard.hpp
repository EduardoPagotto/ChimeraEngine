#pragma once
#include "interfaces.hpp"
#include <map>

namespace ce {
    class Keyboard : public ServiceBase<IKeyboard> {
      private:
        std::map<SDL_Keycode, uint8_t> mapKey;
        uint16_t mod;

      public:
        Keyboard() noexcept = default;
        virtual ~Keyboard() noexcept override = default;

        virtual void setDown(const SDL_KeyboardEvent& event) noexcept override {
            mapKey[event.keysym.sym] = SDL_PRESSED;
            mod = event.keysym.mod;
        }

        virtual void setUp(const SDL_KeyboardEvent& event) noexcept override {
            mapKey[event.keysym.sym] = SDL_RELEASED;
            mod = event.keysym.mod;
        }

        virtual const bool isPressed(const SDL_Keycode& key) noexcept override {

            if (mapKey.contains(key))
                return (mapKey[key] == SDL_PRESSED);

            return false;
        }

        virtual const bool isModPressed(const SDL_Keymod& keyMod) const noexcept override { return (keyMod & mod); }

        virtual const bool getEvent(const SDL_Event& event) noexcept override {

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
} // namespace ce
