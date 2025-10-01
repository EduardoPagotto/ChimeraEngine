#pragma once
#include "ServiceLocator.hpp"
#include <SDL3/SDL.h>
#include <map>

namespace ce {

    /// @brief Pad Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20250604
    class GamePad : public IService {

      private:
        std::map<SDL_JoystickID, SDL_Gamepad*> pads;

      public:
        GamePad() noexcept {
            SDL_InitSubSystem(SDL_INIT_GAMEPAD);
            SDL_SetGamepadEventsEnabled(true);
        }

        virtual ~GamePad() noexcept {
            for (auto i = pads.begin(); i != pads.end(); i++)
                SDL_CloseGamepad(i->second);

            pads.clear();
        }

        // IService base
        std::type_index getTypeIndex() const override { return std::type_index(typeid(GamePad)); }

        const bool getEvent(const SDL_Event& event) noexcept {

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

        SDL_Gamepad* getFirst() noexcept {
            if (pads.begin() != pads.end())
                return pads.begin()->second;

            return nullptr;
        }

        SDL_Gamepad* get(const SDL_JoystickID& pad_id) noexcept {

            if (auto got = pads.find(pad_id); got != pads.end())
                return got->second;

            return nullptr;
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

                    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Gamepad %" SDL_PRIu32 ": %s%s%s VID 0x%.4x, PID 0x%.4x",
                                instance_id, name ? name : "Unknown", path ? ", " : "", path ? path : "",
                                SDL_GetGamepadVendorForID(instance_id), SDL_GetGamepadProductForID(instance_id));

                    char guid[64];
                    SDL_GUIDToString(SDL_GetGamepadGUIDForID(instance_id), guid, sizeof(guid));
                    SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, " guid: %s", guid);

                    if (pads.contains(instance_id))
                        continue;

                    if (SDL_IsGamepad(instance_id)) {
                        SDL_Gamepad* handle = SDL_OpenGamepad(instance_id);

                        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Gamepad id: %d", SDL_GetGamepadID(handle));

                        pads[instance_id] = handle;
                    }
                }
            }
        }

        void removed(const SDL_GamepadDeviceEvent& device) {
            if (pads.contains(device.which)) {
                SDL_CloseGamepad(pads[device.which]);
                pads.erase(device.which);
            }
        }
    };
} // namespace ce
