#include "chimera/core/device/GameControllerManager.hpp"

namespace Chimera {

void GameControllerManager::init(void) {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    SDL_GameControllerEventState(SDL_ENABLE);
}

void GameControllerManager::find(void) {

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

void GameControllerManager::remover(const SDL_ControllerDeviceEvent& device) {
    if (pads.contains(device.which)) {
        SDL_GameControllerClose(pads[device.which]);
        pads.erase(device.which);
    }
}

void GameControllerManager::release(void) {

    for (auto i = pads.begin(); i != pads.end(); i++)
        SDL_GameControllerClose(i->second);

    pads.clear();
}

bool GameControllerManager::getEvent(const SDL_Event& event) {

    switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
            GameControllerManager::find();
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            GameControllerManager::remover(event.cdevice);
            break;
    }

    return false;
}
} // namespace Chimera
