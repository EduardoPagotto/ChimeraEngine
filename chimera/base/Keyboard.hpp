#pragma once
#include "ServiceLocator.hpp"
#include <SDL3/SDL.h>
#include <map>

namespace ce {

    /// @brief Keyboard Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20250527
    class Keyboard : public IService {
      private:
        std::map<SDL_Keycode, bool> mapKey;
        SDL_Keymod mod;

      public:
        Keyboard() noexcept = default;

        virtual ~Keyboard() noexcept override = default;

        // IService base
        std::type_index getTypeIndex() const override { return std::type_index(typeid(Keyboard)); }

        void setDown(const SDL_KeyboardEvent& event) noexcept {
            mapKey[event.key] = true;
            mod = event.mod;
        }

        void setUp(const SDL_KeyboardEvent& event) noexcept {
            mapKey[event.key] = false;
            mod = event.mod;
        }

        const bool isPressed(const SDL_Keycode& key) noexcept {

            if (mapKey.contains(key))
                return (mapKey[key] == true);

            return false;
        }

        const bool isModPressed(const SDL_Keymod& keyMod) const noexcept { return (keyMod & mod); }

        const bool getEvent(const SDL_Event& event) noexcept {

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
