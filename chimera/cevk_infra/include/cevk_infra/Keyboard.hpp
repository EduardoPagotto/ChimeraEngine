#pragma once
#include <SDL3/SDL.h>
#include <map>

namespace ce {

    /// @brief Keyboard Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260731
    class Keyboard {
      public:
        Keyboard() noexcept = default;
        virtual ~Keyboard() noexcept = default;

        void setDown(const SDL_KeyboardEvent& event) noexcept;
        void setUp(const SDL_KeyboardEvent& event) noexcept;
        const bool isPressed(const SDL_Keycode& key) noexcept;
        const bool isModPressed(const SDL_Keymod& keyMod) const noexcept { return (keyMod & mod); }
        const bool getEvent(const SDL_Event& event) noexcept;

      private:
        std::map<SDL_Keycode, bool> mapKey;
        SDL_Keymod mod;
    };
} // namespace ce
