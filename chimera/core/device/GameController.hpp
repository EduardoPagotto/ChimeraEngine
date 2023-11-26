#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {
class GameController {
  public:
    GameController() = default;
    virtual ~GameController() = default;

    inline void init(void) {
        SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
        SDL_GameControllerEventState(SDL_ENABLE);
    }

    inline void release(void) {
        for (auto i = pads.begin(); i != pads.end(); i++)
            SDL_GameControllerClose(i->second);

        pads.clear();
    }

    inline bool getEvent(const SDL_Event& event) {

        switch (event.type) {
            case SDL_CONTROLLERDEVICEADDED:
                GameController::added();
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                GameController::removed(event.cdevice);
                break;
        }

        return false;
    }

    inline SDL_GameController* get(const SDL_JoystickID& pad_id) { return pads.contains(pad_id) ? pads[pad_id] : nullptr; }

  private:
    void added(void) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {

            if (SDL_IsGameController(i)) {
                SDL_GameController* handle = SDL_GameControllerOpen(i);
                SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(handle));

                if (pads.contains(id))
                    continue;

                pads[id] = handle;

                const char* name = SDL_GameControllerName(handle);
                SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Game controller name %d: %s", i, name ? name : "[no name]");

                char* mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(i));
                SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Game controller map: %s", mapping);
                SDL_free(mapping);
            }
        }
    }

    void removed(const SDL_ControllerDeviceEvent& device) {
        if (pads.contains(device.which)) {
            SDL_GameControllerClose(pads[device.which]);
            pads.erase(device.which);
        }
    }
    std::map<SDL_JoystickID, SDL_GameController*> pads;
};
} // namespace Chimera
