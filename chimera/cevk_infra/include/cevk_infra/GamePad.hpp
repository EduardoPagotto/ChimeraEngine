#pragma once
#include <SDL3/SDL.h>
#include <map>

namespace ce {

    /// @brief Pad Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260903
    class GamePad {

      public:
        GamePad();
        virtual ~GamePad() noexcept;
        bool getEvent(const SDL_Event& event) noexcept;
        SDL_Gamepad* getFirst() noexcept;
        SDL_Gamepad* get(const SDL_JoystickID& pad_id) noexcept;

      private:
        void added(void);
        void removed(const SDL_GamepadDeviceEvent& device);

        std::map<SDL_JoystickID, SDL_Gamepad*> pads;
    };
} // namespace ce
