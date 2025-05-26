#pragma once
#include "interfaces.hpp"
#include <map>

namespace ce {
    class Keyboard : public ServiceBase<IKeyboard> {
      private:
        std::map<SDL_Keycode, bool> mapKey;
        SDL_Keymod mod;

      public:
        Keyboard() noexcept = default;
        virtual ~Keyboard() noexcept override = default;

        virtual void setDown(const SDL_KeyboardEvent& event) noexcept override {
            mapKey[event.key] = true;
            mod = event.mod;
        }

        virtual void setUp(const SDL_KeyboardEvent& event) noexcept override {
            mapKey[event.key] = false;
            mod = event.mod;
        }

        virtual const bool isPressed(const SDL_Keycode& key) noexcept override {

            if (mapKey.contains(key))
                return (mapKey[key] == true);

            return false;
        }

        virtual const bool isModPressed(const SDL_Keymod& keyMod) const noexcept override { return (keyMod & mod); }

        virtual const bool getEvent(const SDL_Event& event) noexcept override {

            switch (event.type) {
                case SDL_EVENT_KEY_DOWN:
                    Keyboard::setDown(event.key);
                    break;
                case SDL_EVENT_KEY_UP:
                    Keyboard::setUp(event.key);
                    break;
            }

            return false;
        }
    };
} // namespace ce
