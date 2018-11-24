#include "JoystickState.hpp"
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

double JoystickState::Axis(const Uint8& axis, const double& deadzone,
                           const double& deadzone_at_ends) {

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

std::string JoystickState::GetStatusJoy() {
    // Create a status string for this joystick.

    std::string return_string;
    char cstr[1024] = "";
#ifdef WIN32
    sprintf_s(cstr, 1024, "Joystick %i: %s\n", id, name.c_str());
#else
    sprintf(cstr, "Joystick %i: %s\n", id, name.c_str());
#endif

    return_string += cstr;

    return_string += "Joy axes:";
    bool first_axis = true;
    for (std::map<Uint8, double>::iterator axis_iter = Axes.begin();
         axis_iter != Axes.end(); axis_iter++) {

        if (first_axis) {
            first_axis = false;
        } else {
            return_string += ",";
        }

#ifdef WIN32
        sprintf_s(cstr, 1024, " %i=%.4f", axis_iter->first, axis_iter->second);
#else
        sprintf(cstr, " %i=%.4f", axis_iter->first, axis_iter->second);
#endif
        return_string += cstr;
    }
    return_string += "\n";

    return_string += "Joy buttons down:";
    for (std::map<Uint8, bool>::iterator button_iter = ButtonsDown.begin();
         button_iter != ButtonsDown.end(); button_iter++) {
        if (button_iter->second) {
#ifdef WIN32
            sprintf_s(cstr, 1024, " %i", button_iter->first);
#else
            sprintf(cstr, " %i", button_iter->first);
#endif
            return_string += cstr;
        }
    }
    return_string += "\n";

    return_string += "Joy hats:";
    bool first_hat = true;
    for (std::map<Uint8, Uint8>::iterator hat_iter = Hats.begin(); hat_iter != Hats.end();
         hat_iter++) {
        if (first_hat) {
            first_hat = false;
        } else {
            return_string += ",";
        }
#ifdef WIN32
        sprintf_s(cstr, 1024, " %i=%i%s%s%s%s%s%s", hat_iter->first, hat_iter->second,
                  hat_iter->second ? "(" : "", hat_iter->second & SDL_HAT_UP ? "U" : "",
                  hat_iter->second & SDL_HAT_DOWN ? "D" : "",
                  hat_iter->second & SDL_HAT_LEFT ? "L" : "",
                  hat_iter->second & SDL_HAT_RIGHT ? "R" : "",
                  hat_iter->second ? ")" : "");
#else
        sprintf(cstr, " %i=%i%s%s%s%s%s%s", hat_iter->first, hat_iter->second,
                hat_iter->second ? "(" : "", hat_iter->second & SDL_HAT_UP ? "U" : "",
                hat_iter->second & SDL_HAT_DOWN ? "D" : "",
                hat_iter->second & SDL_HAT_LEFT ? "L" : "",
                hat_iter->second & SDL_HAT_RIGHT ? "R" : "", hat_iter->second ? ")" : "");
#endif
        return_string += cstr;
    }

    return return_string;
}
} // namespace Chimera
