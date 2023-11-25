#include "chimera/core/device/JoystickManager.hpp"

namespace Chimera {

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
                // JoystickManager::debug();
            }
        }
    }
}

void JoystickManager::release(void) {

    for (auto joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter++) {

        if (joy_iter->second.pHandle)
            SDL_JoystickClose(joy_iter->second.pHandle);

        joy_iter->second.pHandle = nullptr;
        joy_iter->second.name = "Disconnected";
    }

    Joysticks.clear();
}

void JoystickManager::setAxisMotion(const SDL_JoyAxisEvent& jaxis) {
    JoystickState* pJoy = &Joysticks[jaxis.which];
    pJoy->axis[jaxis.axis] = jaxis.value;
}

void JoystickManager::setButtonState(const SDL_JoyButtonEvent& jbutton) {
    JoystickState* pJoy = &Joysticks[jbutton.which];
    pJoy->buttonState[jbutton.button] = jbutton.state;
}

void JoystickManager::setHatMotion(const SDL_JoyHatEvent& jhat) {
    JoystickState* pJoy = &Joysticks[jhat.which];
    pJoy->hats[jhat.hat] = jhat.value;
}

void JoystickManager::setBallMotion(const SDL_JoyBallEvent& jball) {
    JoystickState* pJoy = &Joysticks[jball.which];
    pJoy->BallsX[jball.ball] += jball.xrel;
    pJoy->BallsY[jball.ball] += jball.yrel;
}

void JoystickManager::getStatusManager(void) {

    std::string return_string = std::string("Joysticks size: ") + std::to_string(Joysticks.size());
    for (auto joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter++) {
        joy_iter->second.debug();
    }
}

bool JoystickManager::getEvent(const SDL_Event& event) {

    switch (event.type) {

        case SDL_JOYAXISMOTION:
            JoystickManager::setAxisMotion(event.jaxis);
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            JoystickManager::setButtonState(event.jbutton);
            break;
        case SDL_JOYHATMOTION:
            JoystickManager::setHatMotion(event.jhat);
            break;
        case SDL_JOYBALLMOTION:
            JoystickManager::setBallMotion(event.jball);
            break;
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            JoystickManager::find();
            break;
    }

    return false;
}

void JoystickManager::debug() {

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
        if (SDL_Joystick* joystick = SDL_JoystickOpen(i); joystick != nullptr) {
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
} // namespace Chimera
