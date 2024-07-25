#pragma once
#include "interfaces.hpp"
#include <map>

namespace Chimera {
class Joystick : public ServiceBase<IJoystick> {
  private:
    std::map<SDL_JoystickID, SDL_Joystick*> joys;

  public:
    Joystick() noexcept {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
        SDL_JoystickEventState(SDL_ENABLE);
    };

    virtual ~Joystick() noexcept override {
        for (auto i = joys.begin(); i != joys.end(); i++)
            SDL_JoystickClose(i->second);

        joys.clear();
    };

    virtual const bool getEvent(const SDL_Event& event) noexcept override {

        switch (event.type) {
            case SDL_JOYDEVICEADDED:
                this->added();
                break;
            case SDL_JOYDEVICEREMOVED:
                this->removed(event.cdevice);
                break;
        }

        return false;
    }

    virtual SDL_Joystick* get(const SDL_JoystickID& joystick_id) noexcept override {
        return joys.contains(joystick_id) ? joys[joystick_id] : nullptr;
    }

  private:
    void added(void) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {

            if (!SDL_IsGameController(i)) {
                if (SDL_Joystick* handle = SDL_JoystickOpen(i); handle != nullptr) {
                    SDL_JoystickID id = SDL_JoystickInstanceID(handle);

                    if (joys.contains(id))
                        continue;

                    this->joys[id] = handle;

                    const char* joystick_name = SDL_JoystickName(handle);
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick %d: %s", i, joystick_name ? joystick_name : "[no name]");
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick id: %d", SDL_JoystickInstanceID(handle));
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick axes: %d", SDL_JoystickNumAxes(handle));
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick hats: %d", SDL_JoystickNumHats(handle));
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick buttons: %d", SDL_JoystickNumButtons(handle));
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick trackballs: %d", SDL_JoystickNumBalls(handle));

                    char guid[64];
                    SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof(guid));
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, " guid: %s", guid);
                }
            }
        }
    }

    void removed(const SDL_ControllerDeviceEvent& device) {
        if (joys.contains(device.which)) {
            SDL_JoystickClose(joys[device.which]);
            joys.erase(device.which);
        }
    }
};
} // namespace Chimera
