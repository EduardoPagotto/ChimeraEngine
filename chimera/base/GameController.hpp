#pragma once
#include "interfaces.hpp"
#include <map>

namespace ce {

    /// @brief Pad controller of game PAD
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20250401
    class GamePad : public ServiceBase<IGamePad> {

      private:
        std::map<SDL_JoystickID, SDL_Gamepad*> pads;

      public:
        GamePad() noexcept {
            SDL_InitSubSystem(SDL_INIT_GAMEPAD);
            SDL_SetGamepadEventsEnabled(true);
        }

        virtual ~GamePad() noexcept override {
            for (auto i = pads.begin(); i != pads.end(); i++)
                SDL_CloseGamepad(i->second);

            pads.clear();
        }

        virtual const bool getEvent(const SDL_Event& event) noexcept override {

            switch (event.type) {
                case SDL_EVENT_GAMEPAD_ADDED:
                    this->added();
                    break;
                case SDL_EVENT_GAMEPAD_REMOVED:
                    this->removed(event.gdevice);
                    break;
            }

            return false;
        }

        virtual SDL_Gamepad* get(const SDL_JoystickID& pad_id) noexcept override {
            return pads.contains(pad_id) ? pads[pad_id] : nullptr;
        }

      private:
        void added(void) {
            // TODO: TESTA GAMEPAD
            int i, num_joysticks;
            SDL_JoystickID* joysticks = SDL_GetGamepads(&num_joysticks);
            if (joysticks) {
                for (i = 0; i < num_joysticks; ++i) {
                    SDL_JoystickID instance_id = joysticks[i];

                    const char* name = SDL_GetGamepadNameForID(instance_id);
                    const char* path = SDL_GetGamepadPathForID(instance_id);

                    SDL_Log("Gamepad %" SDL_PRIu32 ": %s%s%s VID 0x%.4x, PID 0x%.4x", instance_id,
                            name ? name : "Unknown", path ? ", " : "", path ? path : "",
                            SDL_GetGamepadVendorForID(instance_id), SDL_GetGamepadProductForID(instance_id));

                    if (pads.contains(instance_id))
                        continue;

                    if (SDL_IsGamepad(i)) {
                        SDL_Gamepad* handle = SDL_OpenGamepad(i);
                        pads[instance_id] = handle;
                    }

                    // TODO: Verificar se preciso limpar no release como abaixo
                    // SDL_free(joysticks);
                }
            }

            // TODO: testar e limpar a versao anteiror abaixo
            // for (int i = 0; i < SDL_GetJoysticks(); i++) {

            //     if (SDL_IsGamepad(i)) {
            //         SDL_Gamepad* handle = SDL_OpenGamepad(i);
            //         SDL_JoystickID id = SDL_GetJoystickID(SDL_GetGamepadJoystick(handle));

            //         if (pads.contains(id))
            //             continue;

            //         pads[id] = handle;

            //         const char* name = SDL_GetGamepadName(handle);
            //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Game controller name %d: %s", i, name ? name : "[no
            //         name]");

            //         char* mapping = SDL_GetGamepadMappingForGUID(SDL_GetJoystickGUIDForID(i));
            //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Game controller map: %s", mapping);
            //         SDL_free(mapping);
            //     }
            // }
        }

        void removed(const SDL_GamepadDeviceEvent& device) {
            if (pads.contains(device.which)) {
                SDL_CloseGamepad(pads[device.which]);
                pads.erase(device.which);
            }
        }
    };
} // namespace ce
