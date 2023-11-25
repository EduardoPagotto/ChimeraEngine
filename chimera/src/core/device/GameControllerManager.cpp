#include "chimera/core/device/GameControllerManager.hpp"

namespace Chimera {

void GameControllerManager::init(void) {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);
    SDL_GameControllerEventState(SDL_ENABLE);
}

void GameControllerManager::add(const SDL_ControllerDeviceEvent& device) {

    for (int i = 0; i < SDL_NumJoysticks(); i++) {

        if (SDL_IsGameController(i)) {
            SDL_GameController* pad = SDL_GameControllerOpen(i);
            SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(pad));

            if (pads.contains(id))
                continue;

            pads[id] = pad;

            const char* name = SDL_GameControllerName(pad);
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

    for (auto joy_iter = pads.begin(); joy_iter != pads.end(); joy_iter++) {
        SDL_GameController* handle = joy_iter->second;
        SDL_GameControllerClose(handle);
    }

    pads.clear();
}

bool GameControllerManager::getEvent(const SDL_Event& event) {

    switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
            GameControllerManager::add(event.cdevice);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            GameControllerManager::remover(event.cdevice);
            break;

        case SDL_CONTROLLER_AXIS_LEFTX:
        case SDL_CONTROLLER_AXIS_LEFTY:
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            break;

        case SDL_CONTROLLER_AXIS_RIGHTX:
        case SDL_CONTROLLER_AXIS_RIGHTY:
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            break;

        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            break;
    }

    return false;
}

// void GameControllerManager::setAxisMotion(SDL_JoyAxisEvent* jaxis) {
//     // JoystickState* pJoy = GameControllerManager::select(jaxis->which);
//     // pJoy->axis[jaxis->axis] = jaxis->value;
// }

// void GameControllerManager::setButtonState(SDL_JoyButtonEvent* jbutton) {
//     // JoystickState* pJoy = GameControllerManager::select(jbutton->which);
//     // pJoy->buttonState[jbutton->button] = jbutton->state;
// }

// void GameControllerManager::setHatMotion(SDL_JoyHatEvent* jhat) {
//     // JoystickState* pJoy = GameControllerManager::select(jhat->which);
//     // pJoy->hats[jhat->hat] = jhat->value;
// }

// void GameControllerManager::setBallMotion(SDL_JoyBallEvent* jball) {
//     // JoystickState* pJoy = GameControllerManager::select(jball->which);
//     // pJoy->BallsX[jball->ball] += jball->xrel;
//     // pJoy->BallsY[jball->ball] += jball->yrel;
// }

SDL_GameController* GameControllerManager::select(const SDL_JoystickID& joystick_id) {

    // auto joy_iter = pads.find(joystick_id);
    // if (joy_iter != pads.end()) {
    //     return &joy_iter->second;
    // }

    return nullptr;
}

void GameControllerManager::getStatusManager(void) {

    // std::string return_string = std::string("pads size: ") + std::to_string(pads.size());
    // for (auto joy_iter = pads.begin(); joy_iter != pads.end(); joy_iter++) {
    //     joy_iter->second.debug();
    // }
}

void GameControllerManager::debug() {

    // for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    //     const char* name = SDL_JoystickNameForIndex(i);
    //     SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick index %d: %s", i, name ? name : "[no name]");
    //     // This much can be done without opening the controller
    //     if (SDL_IsGameController(i)) {
    //         char* mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(i));
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "game controller: %s", mapping);
    //         SDL_free(mapping);
    //     } else {
    //         char guid[64];
    //         SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof(guid));
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, " guid: %s", guid);
    //     }

    //     // For anything else we have to open
    //     if (SDL_Joystick* joystick = SDL_JoystickOpen(i); joystick != nullptr) {
    //         // Not the same as a device index!
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick instance id: %d", SDL_JoystickInstanceID(joystick));
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick axes: %d", SDL_JoystickNumAxes(joystick));
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick hats: %d", SDL_JoystickNumHats(joystick));
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick buttons: %d", SDL_JoystickNumButtons(joystick));
    //         // I've _never_ seen this non-zero, if anyone has lemme know!
    //         SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick trackballs: %d", SDL_JoystickNumBalls(joystick));
    //         SDL_JoystickClose(joystick);
    //     }
    // }
}
} // namespace Chimera
