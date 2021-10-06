#include "chimera/core/io/JoystickManager.hpp"

namespace Chimera::Core {

std::map<SDL_JoystickID, JoystickState> JoystickManager::Joysticks;

void JoystickManager::init(void) {
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
}

void JoystickManager::find(void) {

    for (int i = 0; i < SDL_NumJoysticks(); i++) {

        if (((Joysticks.size() == 0)) || (!Joysticks[i].pHandle)) {

            Joysticks[i].id = i;
            Joysticks[i].pHandle = SDL_JoystickOpen(i);

            if (Joysticks[i].pHandle) {

                const char* joystick_name = SDL_JoystickName(Joysticks[i].pHandle);
                Joysticks[i].name = joystick_name ? joystick_name : "Joystick";
            }
        }
    }
}

void JoystickManager::release(void) {

    for (auto joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter++) {

        if (joy_iter->second.pHandle)
            SDL_JoystickClose(joy_iter->second.pHandle);

        joy_iter->second.pHandle = NULL;
        joy_iter->second.name = "Disconnected";
    }

    Joysticks.clear();
}

void JoystickManager::setAxisMotion(SDL_JoyAxisEvent* jaxis) {
    JoystickState* pJoy = JoystickManager::select(jaxis->which);
    pJoy->axis[jaxis->axis] = jaxis->value;
}

void JoystickManager::setButtonState(SDL_JoyButtonEvent* jbutton) {
    JoystickState* pJoy = JoystickManager::select(jbutton->which);
    pJoy->buttonState[jbutton->button] = jbutton->state;
}

void JoystickManager::setHatMotion(SDL_JoyHatEvent* jhat) {
    JoystickState* pJoy = JoystickManager::select(jhat->which);
    pJoy->hats[jhat->hat] = jhat->value;
}

void JoystickManager::setBallMotion(SDL_JoyBallEvent* jball) {
    JoystickState* pJoy = JoystickManager::select(jball->which);
    pJoy->BallsX[jball->ball] += jball->xrel;
    pJoy->BallsY[jball->ball] += jball->yrel;
}

JoystickState* JoystickManager::select(const SDL_JoystickID& joystick_id) {

    auto joy_iter = Joysticks.find(joystick_id);
    if (joy_iter != Joysticks.end()) {
        return &joy_iter->second;
    }

    return nullptr;
}

void JoystickManager::getStatusManager(void) {

    std::string return_string = std::string("Joysticks size: ") + std::to_string(Joysticks.size());
    for (auto joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter++) {
        joy_iter->second.debug();
    }
}

void JoystickManager::debug() {

    SDL_Joystick* joystick;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        const char* name = SDL_JoystickNameForIndex(i);
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick index %d: %s", i, name ? name : "[no name]");
        // This much can be done without opening the controller
        if (SDL_IsGameController(i)) {
            char* mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(i));
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "game controller: %s", mapping);
            SDL_free(mapping);
        } else {
            char guid[64];
            SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof(guid));
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, " guid: %s", guid);
        }

        // For anything else we have to open
        joystick = SDL_JoystickOpen(i);
        if (joystick != nullptr) {
            // Not the same as a device index!
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick instance id: %d", SDL_JoystickInstanceID(joystick));
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick axes: %d", SDL_JoystickNumAxes(joystick));
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick hats: %d", SDL_JoystickNumHats(joystick));
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick buttons: %d", SDL_JoystickNumButtons(joystick));
            // I've _never_ seen this non-zero, if anyone has lemme know!
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick trackballs: %d", SDL_JoystickNumBalls(joystick));
            SDL_JoystickClose(joystick);
        }
    }
}
} // namespace Chimera::Core
