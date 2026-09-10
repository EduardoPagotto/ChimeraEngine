#pragma once
#include <SDL3/SDL.h>
#include <unordered_map>

namespace ce {

    /// @brief Keyboard Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260907
    class Keyboard {
      public:
        Keyboard() noexcept {
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Keyboard init ok");
            mapKey.reserve(4);
        };
        virtual ~Keyboard() noexcept = default;

        void setDown(const SDL_KeyboardEvent& event) noexcept {
            mapKey[event.key] = true;
            mod = event.mod;
        }

        void setUp(const SDL_KeyboardEvent& event) noexcept {
            mapKey[event.key] = false;
            mod = event.mod;
        }

        bool isPressed(const SDL_Keycode& key) noexcept {
            if (mapKey.contains(key)) {
                return (mapKey[key]);
            }

            return false;
        }

        bool isModPressed(const SDL_Keymod& keyMod) const noexcept { return ((keyMod & mod) != 0); }

        void clean() {
            mod = SDL_KMOD_NONE;
            mapKey.clear();
        }

      private:
        std::unordered_map<SDL_Keycode, bool> mapKey;
        SDL_Keymod mod{SDL_KMOD_NONE};
    };
} // namespace ce
