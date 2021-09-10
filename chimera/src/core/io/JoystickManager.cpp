#include "chimera/core/io/JoystickManager.hpp"

namespace Chimera::IO {

JoystickManager::JoystickManager(void) { Initialized = false; }

JoystickManager::~JoystickManager(void) {

    if (Initialized) {
        ReleaseJoysticks();
        SDL_JoystickEventState(SDL_DISABLE);
        SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    }

    Initialized = false;
}

void JoystickManager::Initialize(void) {

    if (!Initialized)
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    SDL_JoystickEventState(SDL_ENABLE);
    Initialized = true;
}

void JoystickManager::FindJoysticks(void) {

    if (!Initialized)
        return;

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

void JoystickManager::ReleaseJoysticks(void) {

    if (!Initialized)
        return;

    for (std::map<uint8_t, JoystickState>::iterator joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter++) {

        if (joy_iter->second.pHandle)
            SDL_JoystickClose(joy_iter->second.pHandle);

        joy_iter->second.pHandle = NULL;
        joy_iter->second.name = "Disconnected";
    }

    Joysticks.clear();
}

JoystickState* JoystickManager::setAxisMotion(SDL_JoyAxisEvent* jaxis) {
    JoystickState* pJoy = this->getJoystickState(jaxis->which);
    pJoy->axis[jaxis->axis] = jaxis->value;
    return pJoy;
}

JoystickState* JoystickManager::setButtonState(SDL_JoyButtonEvent* jbutton) { // FIXME: melhorar
    JoystickState* pJoy = this->getJoystickState(jbutton->which);
    pJoy->buttonState[jbutton->button] = jbutton->state;
    return pJoy;
}

JoystickState* JoystickManager::setHatMotion(SDL_JoyHatEvent* jhat) {
    JoystickState* pJoy = this->getJoystickState(jhat->which);
    pJoy->hats[jhat->hat] = jhat->value;
    return pJoy;
}

JoystickState* JoystickManager::setBallMotion(SDL_JoyBallEvent* jball) {
    JoystickState* pJoy = this->getJoystickState(jball->which);
    pJoy->BallsX[jball->ball] += jball->xrel;
    pJoy->BallsY[jball->ball] += jball->yrel;
    return pJoy;
}

JoystickState* JoystickManager::getJoystickState(const uint8_t& joystick_id) {

    std::map<uint8_t, JoystickState>::iterator joy_iter = Joysticks.find(joystick_id);
    if (joy_iter != Joysticks.end()) {
        return &joy_iter->second;
    }

    return nullptr;
}

void JoystickManager::GetStatusManager(void) {

    std::string return_string = std::string("Joysticks size: ") + std::to_string(Joysticks.size());
    for (std::map<uint8_t, JoystickState>::iterator joy_iter = Joysticks.begin(); joy_iter != Joysticks.end(); joy_iter++) {
        joy_iter->second.debug();
    }
}

void JoystickManager::DebugDadosJoystick() {

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
} // namespace Chimera::IO
