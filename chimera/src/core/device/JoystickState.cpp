#include "chimera/core/device/JoystickState.hpp"
#include <cmath>

namespace Chimera {

JoystickState::JoystickState() : id(255), pHandle(nullptr), name("none") {}

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

uint8_t JoystickState::getButtonState(const uint8_t& indice) {
    auto button_iter = buttonState.find(indice);
    if (button_iter != buttonState.end())
        return button_iter->second;

    return SDL_RELEASED;
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

    for (auto button_iter = buttonState.begin(); button_iter != buttonState.end(); button_iter++) {
        auto val = button_iter->second;
        if (val == SDL_PRESSED)
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy buttons %d State: PRESSED", button_iter->first);
        else if (val == SDL_RELEASED)
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy buttons %d State: RELEASE", button_iter->first);
        else
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy buttons %d unkwow: %d", button_iter->first, val);
    }

    std::string tot = "";
    for (auto hat_iter = hats.begin(); hat_iter != hats.end(); hat_iter++) {

        tot += hat_iter->second & SDL_HAT_UP ? "U" : "";
        tot += hat_iter->second & SDL_HAT_DOWN ? "D" : "";
        tot += hat_iter->second & SDL_HAT_LEFT ? "L" : "";
        tot += hat_iter->second & SDL_HAT_RIGHT ? "R" : "";
        if (tot.size() == 0)
            tot += "C";

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Joy hats: %d %d [ %s ]", hat_iter->first, hat_iter->second, tot.c_str());
    }
}
} // namespace Chimera
