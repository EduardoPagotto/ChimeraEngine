#include "chimera/core/JoystickState.hpp"
#include <cmath>

namespace Chimera {

JoystickState::JoystickState() : id(255), pJoystick(nullptr), name("none") {}

void JoystickState::TrackEvent(SDL_Event* event) {
    // Update joystick axis and button status.
    switch (event->type) {
        case SDL_JOYAXISMOTION:
            axis[event->jaxis.axis] = event->jaxis.value;
            break;
        case SDL_JOYBUTTONDOWN:
            button[event->jbutton.button] = true;
            buttonState[event->jbutton.button] = event->jbutton.state;
            break;
        case SDL_JOYBUTTONUP:
            button[event->jbutton.button] = false;
            buttonState[event->jbutton.button] = event->jbutton.state;
            break;
        case SDL_JOYHATMOTION:
            hats[event->jhat.hat] = event->jhat.value;
            break;
        case SDL_JOYBALLMOTION:
            BallsX[event->jball.ball] += event->jball.xrel;
            BallsY[event->jball.ball] += event->jball.yrel;
            break;
    }
}

int16_t JoystickState::getAxis(const uint8_t& index, const int16_t& deadzone, const int16_t& deadzone_at_ends) {

    auto axis_iter = axis.find(index);
    if (axis_iter != axis.end()) {

        int16_t value = axis_iter->second;

        if (fabs(value) < deadzone)
            return 0;

        // else if (value + deadzone_at_ends > 1.0f)
        //     return 1.0f;
        // else if (value - deadzone_at_ends < -1.0f)
        //     return -1.0f;
        // else {
        //     // Reclaim the range of values lost to the deadzones.
        //     if (value > 0.) {
        //         value -= deadzone;
        //     } else {
        //         value += deadzone;
        //     }

        //     value /= (1.0f - deadzone - deadzone_at_ends);
        // }

        return value;
    }

    return 0;
}

bool JoystickState::getButton(const uint8_t& indice) {

    auto button_iter = button.find(indice);
    if (button_iter != button.end())
        return button_iter->second;

    return false;
}

uint8_t JoystickState::getButtonState(const uint8_t& indice) {
    auto button_iter = buttonState.find(indice);
    if (button_iter != buttonState.end())
        return button_iter->second;

    return false;
}

uint8_t JoystickState::getHat(const uint8_t& hat) {
    // Check the direction of a hat switch.

    auto hat_iter = hats.find(hat);
    if (hat_iter != hats.end()) {
        return hat_iter->second;
    }

    return 0;
}

void JoystickState::debug() {

    // Log do status de joystick
    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joystick (%d): %s", id, name.c_str());

    for (auto axis_iter = axis.begin(); axis_iter != axis.end(); axis_iter++)
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy axis: %d %d", axis_iter->first, axis_iter->second);

    for (auto button_iter = button.begin(); button_iter != button.end(); button_iter++) {
        if (button_iter->second)
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy buttons down: %d", button_iter->first);
    }

    std::string tot = "";
    for (auto hat_iter = hats.begin(); hat_iter != hats.end(); hat_iter++) {
        if (hat_iter->second) {
            tot += hat_iter->second & SDL_HAT_UP ? "U" : "";
            tot += hat_iter->second & SDL_HAT_DOWN ? "D" : "";
            tot += hat_iter->second & SDL_HAT_LEFT ? "L" : "";
            tot += hat_iter->second & SDL_HAT_RIGHT ? "R" : "";
        }
        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy hats: %d %d [ %s ]", hat_iter->first, hat_iter->second, tot.c_str());
    }
}
} // namespace Chimera
