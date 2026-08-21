#pragma once

#include "cevk_infra/GamePad.hpp"
#include "cevk_infra/Joystick.hpp"
#include "cevk_infra/Keyboard.hpp"
#include "cevk_infra/Mouse.hpp"
namespace ce {

    class InputManager {
      public:
        explicit InputManager() {}
        virtual ~InputManager() = default;

      private:
        Mouse mouse;
        Keyboard keyboard;
        Joystick joystick;
        GamePad gamePad;
    };
} // namespace ce
