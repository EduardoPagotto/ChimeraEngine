#pragma once
#include "interfaces.hpp"
#include <map>

namespace ce {

    class Joystick : public ServiceBase<IJoystick> {

      private:
        std::map<SDL_JoystickID, SDL_Joystick*> joys;

      public:
        Joystick() noexcept {
            SDL_InitSubSystem(SDL_INIT_JOYSTICK);
            SDL_SetJoystickEventsEnabled(true);
        };

        virtual ~Joystick() noexcept override {
            for (auto i = joys.begin(); i != joys.end(); i++)
                SDL_CloseJoystick(i->second);

            joys.clear();
        };

        virtual const bool getEvent(const SDL_Event& event) noexcept override {

            switch (event.type) {
                case SDL_EVENT_JOYSTICK_ADDED:
                    this->added();
                    break;
                case SDL_EVENT_JOYSTICK_REMOVED:
                    this->removed(event.jdevice);
                    break;
            }

            return false;
        }

        virtual SDL_Joystick* get(const SDL_JoystickID& joystick_id) noexcept override {
            return joys.contains(joystick_id) ? joys[joystick_id] : nullptr;
        }

      private:
        // TODO: TESTAR JOYSTICK
        void added(void) {

            int i, num_joysticks;
            SDL_JoystickID* joysticks = SDL_GetJoysticks(&num_joysticks);
            if (joysticks) {
                for (i = 0; i < num_joysticks; ++i) {
                    SDL_JoystickID instance_id = joysticks[i];
                    const char* name = SDL_GetJoystickNameForID(instance_id);
                    const char* path = SDL_GetJoystickPathForID(instance_id);

                    SDL_Log("Joystick %" SDL_PRIu32 ": %s%s%s VID 0x%.4x, PID 0x%.4x", instance_id,
                            name ? name : "Unknown", path ? ", " : "", path ? path : "",
                            SDL_GetJoystickVendorForID(instance_id), SDL_GetJoystickProductForID(instance_id));

                    if (joys.contains(instance_id))
                        continue;

                    this->joys[instance_id] = SDL_OpenJoystick(i);
                }
                // SDL_free(joysticks);
            }
            // SDL_QuitSubSystem(SDL_INIT_JOYSTICK);

            // for (int i = 0; i < SDL_NumJoysticks(); i++) {

            //     if (!SDL_IsGameController(i)) {
            //         if (SDL_Joystick* handle = SDL_JoystickOpen(i); handle != nullptr) {
            //             SDL_JoystickID id = SDL_JoystickInstanceID(handle);

            //             if (joys.contains(id))
            //                 continue;

            //             this->joys[id] = handle;

            //             const char* joystick_name = SDL_JoystickName(handle);
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick %d: %s", i,
            //                          joystick_name ? joystick_name : "[no name]");
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick id: %d", SDL_JoystickInstanceID(handle));
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick axes: %d", SDL_JoystickNumAxes(handle));
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick hats: %d", SDL_JoystickNumHats(handle));
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick buttons: %d", SDL_JoystickNumButtons(handle));
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick trackballs: %d",
            //             SDL_JoystickNumBalls(handle));

            //             char guid[64];
            //             SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof(guid));
            //             SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, " guid: %s", guid);
            //         }
            //     }
            // }
        }

        void removed(const SDL_JoyDeviceEvent& device) {
            if (joys.contains(device.which)) {
                SDL_CloseJoystick(joys[device.which]);
                joys.erase(device.which);
            }
        }
    };
} // namespace ce
