#include "chimera/core/JoystickState.hpp"
#include "chimera/core/Logger.hpp"
#include <cmath>

namespace Chimera {

JoystickState::JoystickState() : id(255), pJoystick(nullptr), name("JoyDefault") {}

// JoystickState::JoystickState( Uint8 idJoy, SDL_Joystick *joystick, std::string nameJoy)
// : id(idJoy) , pJoystick(joystick), name(nameJoy) { }

void JoystickState::TrackEvent(SDL_Event* event) {
    // Update joystick axis and button status.
    switch (event->type) {
        case SDL_JOYAXISMOTION:
            Axes[event->jaxis.axis] = AxisScale(event->jaxis.value);
            break;
        case SDL_JOYBUTTONDOWN:
            ButtonsDown[event->jbutton.button] = true;
            break;
        case SDL_JOYBUTTONUP:
            ButtonsDown[event->jbutton.button] = false;
            break;
        case SDL_JOYHATMOTION:
            Hats[event->jhat.hat] = event->jhat.value;
            break;
        case SDL_JOYBALLMOTION:
            BallsX[event->jball.ball] += event->jball.xrel;
            BallsY[event->jball.ball] += event->jball.yrel;
            break;
    }
}

double JoystickState::Axis(const Uint8& axis, const double& deadzone, const double& deadzone_at_ends) {

    std::map<Uint8, double>::iterator axis_iter = Axes.find(axis);
    if (axis_iter != Axes.end()) {

        double value = axis_iter->second;

        if (fabs(value) < deadzone)
            return 0.0f;
        else if (value + deadzone_at_ends > 1.0f)
            return 1.0f;
        else if (value - deadzone_at_ends < -1.0f)
            return -1.0f;
        else {
            // Reclaim the range of values lost to the deadzones.
            if (value > 0.) {
                value -= deadzone;
            } else {
                value += deadzone;
            }

            value /= (1.0f - deadzone - deadzone_at_ends);
        }

        return value;
    }

    return 0.0f;
}

bool JoystickState::ButtonDown(const Uint8& button) {

    std::map<Uint8, bool>::iterator button_iter = ButtonsDown.find(button);
    if (button_iter != ButtonsDown.end())
        return button_iter->second;

    return false;
}

Uint8 JoystickState::Hat(const Uint8& hat) {
    // Check the direction of a hat switch.

    std::map<Uint8, Uint8>::iterator hat_iter = Hats.find(hat);
    if (hat_iter != Hats.end()) {
        return hat_iter->second;
    }

    return 0;
}

void JoystickState::GetStatusJoy() {

    // Log do status de joystick
    Logger* pLog = Logger::get();

    pLog->debug(std::string("Joystick (" + std::to_string(id) + "): " + name));
    for (std::map<Uint8, double>::iterator axis_iter = Axes.begin(); axis_iter != Axes.end(); axis_iter++)
        pLog->debug(
            std::string("Joy axes:" + std::to_string(axis_iter->first) + " " + std::to_string(axis_iter->second)));

    for (std::map<Uint8, bool>::iterator button_iter = ButtonsDown.begin(); button_iter != ButtonsDown.end();
         button_iter++) {
        if (button_iter->second)
            pLog->debug(std::string("Joy buttons down: " + std::to_string(button_iter->first)));
    }

    std::string tot = "";
    for (std::map<Uint8, Uint8>::iterator hat_iter = Hats.begin(); hat_iter != Hats.end(); hat_iter++) {
        if (hat_iter->second) {
            tot += hat_iter->second & SDL_HAT_UP ? "U" : "";
            tot += hat_iter->second & SDL_HAT_DOWN ? "D" : "";
            tot += hat_iter->second & SDL_HAT_LEFT ? "L" : "";
            tot += hat_iter->second & SDL_HAT_RIGHT ? "R" : "";
        }
        pLog->debug(std::string(std::string("Joy hats:") + std::to_string(hat_iter->first) + " " +
                                std::to_string(hat_iter->second) + " [" + tot + "]"));
    }
}
} // namespace Chimera
