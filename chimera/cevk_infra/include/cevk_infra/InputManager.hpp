#pragma once

#include "cevk_infra/GamePad.hpp"
#include "cevk_infra/Joystick.hpp"
#include "cevk_infra/Keyboard.hpp"
#include "cevk_infra/Mouse.hpp"
#include <SDL3/SDL_stdinc.h>
#include <optional>
namespace ce {

    // 1. Criamos um "Trait" para identificar quais enums aceitam bitwise
    template <typename T>
    struct EnableBitmaskOperators : std::false_type {};

    // 2. Criamos os operadores genéricos usando Concepts (C++20)
    template <typename T>
        requires EnableBitmaskOperators<T>::value
    constexpr T operator|(T lhs, T rhs) {
        using underlying = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
    }

    template <typename T>
        requires EnableBitmaskOperators<T>::value
    constexpr T operator&(T lhs, T rhs) {
        using underlying = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
    }

    // 1. Definição do enum especificando o tipo subjacente (ex: uint8_t para economizar memória)
    enum class InputEnable : uint8_t {
        None = 0,          // 0000
        Mouse = 1 << 0,    // 0001
        Keyboard = 1 << 1, // 0010
        Joystick = 1 << 2, // 0100
        Gamepad = 1 << 3,  // 1000
    };

    // 4. Ativamos explicitamente os operadores para o nosso enum
    template <>
    struct EnableBitmaskOperators<InputEnable> : std::true_type {};

    struct InputManager {
        InputManager(InputEnable input, bool executeEventChild) : executeEventChild(executeEventChild) {
            using enum InputEnable;
            if ((input & Mouse) != None) {
                mouse.emplace();
            }
            if ((input & Keyboard) != None) {
                keyboard.emplace();
            }
            if ((input & Joystick) != None) {
                joystick.emplace();
            }
            if ((input & Gamepad) != None) {
                gamePad.emplace();
            }
        }

        void update() {
            if (this->keyboard.has_value()) {
                this->keyboard->clean();
            }

            if (this->mouse.has_value()) {
                this->mouse->clear();
            }
        }

        std::optional<Mouse> mouse = std::nullopt;
        std::optional<Keyboard> keyboard = std::nullopt;
        std::optional<Joystick> joystick = std::nullopt;
        std::optional<GamePad> gamePad = std::nullopt;
        bool paused{false};
        bool executeEventChild{false};
    };
} // namespace ce
