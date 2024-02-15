#pragma once
#include <SDL2/SDL.h>
#include <map>

namespace Chimera {
class GameController {
  private:
    std::map<SDL_JoystickID, SDL_GameController*> m_pads;

  public:
    GameController() {
        SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
        SDL_GameControllerEventState(SDL_ENABLE);
    }
    virtual ~GameController() {
        for (auto i = m_pads.begin(); i != m_pads.end(); i++)
            SDL_GameControllerClose(i->second);

        m_pads.clear();
    }

    inline bool getEvent(const SDL_Event& event) {

        switch (event.type) {
            case SDL_CONTROLLERDEVICEADDED:
                this->added();
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                this->removed(event.cdevice);
                break;
        }

        return false;
    }

    inline SDL_GameController* get(const SDL_JoystickID& pad_id) { return m_pads.contains(pad_id) ? m_pads[pad_id] : nullptr; }

  private:
    void added(void) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {

            if (SDL_IsGameController(i)) {
                SDL_GameController* handle = SDL_GameControllerOpen(i);
                SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(handle));

                if (m_pads.contains(id))
                    continue;

                m_pads[id] = handle;

                const char* name = SDL_GameControllerName(handle);
                SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Game controller name %d: %s", i, name ? name : "[no name]");

                char* mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(i));
                SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Game controller map: %s", mapping);
                SDL_free(mapping);
            }
        }
    }

    void removed(const SDL_ControllerDeviceEvent& device) {
        if (m_pads.contains(device.which)) {
            SDL_GameControllerClose(m_pads[device.which]);
            m_pads.erase(device.which);
        }
    }
};
} // namespace Chimera
