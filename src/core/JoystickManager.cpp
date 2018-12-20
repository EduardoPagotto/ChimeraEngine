#include "chimera/core/JoystickManager.hpp"
#include "chimera/core/Logger.hpp"

namespace Chimera {

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

        if (!Joysticks[i].pJoystick) {

            Joysticks[i].id = i;
            Joysticks[i].pJoystick = SDL_JoystickOpen(i);

            if (Joysticks[i].pJoystick) {

                const char* joystick_name = SDL_JoystickName(Joysticks[i].pJoystick);
                Joysticks[i].name = joystick_name ? joystick_name : "Joystick";
            }
        }
    }
}

void JoystickManager::ReleaseJoysticks(void) {

    if (!Initialized)
        return;

    for (std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.begin(); joy_iter != Joysticks.end();
         joy_iter++) {

        if (joy_iter->second.pJoystick)
            SDL_JoystickClose(joy_iter->second.pJoystick);

        joy_iter->second.pJoystick = NULL;
        joy_iter->second.name = "Disconnected";
    }

    Joysticks.clear();
}

bool JoystickManager::TrackEvent(SDL_Event* event) {
    Uint8 id = 255;
    switch (event->type) {
        case SDL_JOYAXISMOTION:
            id = event->jaxis.which;
            break;
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            id = event->jbutton.which;
            break;
        case SDL_JOYHATMOTION:
            id = event->jhat.which;
            break;
        case SDL_JOYBALLMOTION:
            id = event->jball.which;
            break;
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            FindJoysticks();
            break;
        default:
            return false;
    }

    Joysticks[id].id = id;
    Joysticks[id].TrackEvent(event);
    return true;
}

JoystickState* JoystickManager::getJoystickState(const int& joystick_id) {

    std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.find(joystick_id);
    if (joy_iter != Joysticks.end()) {
        return &joy_iter->second;
    }

    return nullptr;
}

void JoystickManager::GetStatusManager(void) {
    // TODO: Testar!!!!
    // Create a status string for all joysticks.
    std::string return_string = std::string("Joysticks size: ") + std::to_string(Joysticks.size());
    for (std::map<Uint8, JoystickState>::iterator joy_iter = Joysticks.begin(); joy_iter != Joysticks.end();
         joy_iter++) {
        joy_iter->second.GetStatusJoy();
    }
}

void JoystickManager::DebugDadosJoystick() {
    // TODO: Testar!!!!
    Logger* pLog = Logger::get();
    SDL_Joystick* joystick;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        const char* name = SDL_JoystickNameForIndex(i);
        pLog->debug("Joystick index %d: %s", i, name ? name : "[no name]");
        // This much can be done without opening the controller
        if (SDL_IsGameController(i)) {
            char* mapping = SDL_GameControllerMappingForGUID(SDL_JoystickGetDeviceGUID(i));
            pLog->debug("game controller: %s", mapping);
            SDL_free(mapping);
        } else {
            char guid[64];
            SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof(guid));
            pLog->debug(" guid: %s", guid);
        }

        // For anything else we have to open
        joystick = SDL_JoystickOpen(i);
        if (joystick != nullptr) {
            // Not the same as a device index!
            pLog->debug("Joystick instance id: %d", SDL_JoystickInstanceID(joystick));
            pLog->debug("Joystick axes: %d", SDL_JoystickNumAxes(joystick));
            pLog->debug("Joystick hats: %d", SDL_JoystickNumHats(joystick));
            pLog->debug("Joystick buttons: %d", SDL_JoystickNumButtons(joystick));
            // I've _never_ seen this non-zero, if anyone has lemme know!
            pLog->debug("Joystick trackballs: %d", SDL_JoystickNumBalls(joystick));
            SDL_JoystickClose(joystick);
        }
    }
}
} // namespace Chimera
