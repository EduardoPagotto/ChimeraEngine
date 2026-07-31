#pragma once
#include <SDL3/SDL.h>
#include <map>

namespace ce {

    /// @brief Joystic Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260731
    class Joystick {

      public:
        Joystick() noexcept;
        virtual ~Joystick() noexcept;

        const bool getEvent(const SDL_Event& event) noexcept;
        SDL_Joystick* get(const SDL_JoystickID& joystick_id) noexcept;

      private:
        // TODO: TESTAR JOYSTICK
        void added(void);
        void removed(const SDL_JoyDeviceEvent& device);

        std::map<SDL_JoystickID, SDL_Joystick*> joys;
    };
} // namespace ce
