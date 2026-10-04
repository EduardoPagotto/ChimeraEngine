#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include <format>
#include <glm/glm.hpp>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace ce {

    /// @brief Pad Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20261004
    class Gamepad {
      public:
        enum class ButtonState {
            None,
            Pressed, // Acabou de ser pressionado neste frame
            Held,    // Sendo segurado por 2 ou mais frames
            Released // Acabou de ser solto neste frame
        };

        struct AxixConfig {
            float deadZoneLeft = 0.15F;
            float deadZoneRight = 0.15F;
            float deadZoneTrigger = 0.15F;
        };

        explicit Gamepad() {

            if (!SDL_InitSubSystem(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)) {
                // if (!SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_JOYSTICK)) {
                throw std::runtime_error(std::format("Erro SDL_INIT_GAMEPAD: {}", SDL_GetError()));
            }
            // SDL_SetGamepadEventsEnabled(true);
            SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Gamepad init ok");
        }

        virtual ~Gamepad() {
            for (auto& [id, gamepad] : connected_gamepads_) {
                SDL_CloseGamepad(gamepad);
            }
            connected_gamepads_.clear();
        }

        // Associa um Gamepad físico (via Instance ID) a um Jogador lógico
        void assign_gamepad_to_player(int player_index, SDL_JoystickID instance_id) {
            player_mappings_[player_index].push_back(instance_id);
        }

        // Atualiza os estados físicos - Chame uma vez no início do seu Game Loop
        bool handle_event(const SDL_Event& event) noexcept { // NOLINT

            if (event.type == SDL_EVENT_GAMEPAD_ADDED) {

                SDL_Gamepad* gamepad = SDL_OpenGamepad(event.gdevice.which);
                if (gamepad != nullptr) {
                    SDL_JoystickID id = SDL_GetGamepadID(gamepad);
                    connected_gamepads_[id] = gamepad;
                    // Por padrão, joga novos controles para o Player 0 (Customizável)
                    assign_gamepad_to_player(0, id);

                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Gamepad conectado ID: %d", id);
                    get_info_pad(id);
                }

            } else if (event.type == SDL_EVENT_GAMEPAD_REMOVED) {

                SDL_JoystickID id = event.gdevice.which;
                if (connected_gamepads_.contains(id)) {
                    SDL_CloseGamepad(connected_gamepads_[id]);
                    connected_gamepads_.erase(id);
                    // Remove mapeamento do jogador
                    for (auto& [player, list] : player_mappings_) {
                        std::erase(list, id);
                    }
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Gamepad desconectado ID: %d", id);
                }
            } else {
                return false;
            }

            return true;
        }

        static void get_info_pad(const SDL_JoystickID& instance_id) {

            const char* name = SDL_GetGamepadNameForID(instance_id);
            const char* path = SDL_GetGamepadPathForID(instance_id);

            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, "Gamepad %" SDL_PRIu32 ": %s%s%s VID 0x%.4x, PID 0x%.4x ", instance_id,
                        (name != nullptr) ? name : "Unknown", (path != nullptr) ? ", " : "",
                        (path != nullptr) ? path : "", SDL_GetGamepadVendorForID(instance_id),
                        SDL_GetGamepadProductForID(instance_id));

            char guid[64];
            SDL_GUIDToString(SDL_GetGamepadGUIDForID(instance_id), guid, sizeof(guid));
            SDL_LogInfo(SDL_LOG_CATEGORY_INPUT, " guid: %s", guid);
        }

        void update_continuous_input() {

            for (auto& [id, gamepad] : connected_gamepads_) {

                auto& b_states = button_states_[id];

                for (int b = SDL_GAMEPAD_BUTTON_SOUTH; b < SDL_GAMEPAD_BUTTON_COUNT; ++b) {

                    auto button = static_cast<SDL_GamepadButton>(b);
                    bool is_down = SDL_GetGamepadButton(gamepad, button);

                    ButtonState& current = b_states[button];

                    if (is_down) {

                        if (current == ButtonState::None || current == ButtonState::Released) {
                            current = ButtonState::Pressed;

                        } else if (current == ButtonState::Pressed) {
                            current = ButtonState::Held;
                        }

                    } else {

                        if (current == ButtonState::Pressed || current == ButtonState::Held) {
                            current = ButtonState::Released;

                        } else if (current == ButtonState::Released) {
                            current = ButtonState::None;
                        }
                    }
                }
            }
        }

        // --- API de Consulta por Jogador (Varre todos os controles atribuídos a ele) ---

        ButtonState get_button_state(int player_index, SDL_GamepadButton button) const {
            if (!player_mappings_.contains(player_index)) {
                return ButtonState::None;
            }

            bool any_pressed = false;
            bool any_held = false;
            bool any_released = false;

            for (SDL_JoystickID id : player_mappings_.at(player_index)) {
                if (!button_states_.contains(id) || !button_states_.at(id).contains(button)) {
                    continue;
                }

                ButtonState state = button_states_.at(id).at(button);
                if (state == ButtonState::Pressed) {
                    any_pressed = true;
                }
                if (state == ButtonState::Held) {
                    any_held = true;
                }
                if (state == ButtonState::Released) {
                    any_released = true;
                }
            }

            if (any_pressed) {
                return ButtonState::Pressed;
            }

            if (any_held) {
                return ButtonState::Held;
            }

            if (any_released) {
                return ButtonState::Released;
            }

            return ButtonState::None;
        }

        glm::vec2 get_left_stick(int player_index, const AxixConfig& config) const {
            return get_normalized_stick(player_index, SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY,
                                        config.deadZoneLeft);
        }

        glm::vec2 get_right_stick(int player_index, const AxixConfig& config) const {
            return get_normalized_stick(player_index, SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY,
                                        config.deadZoneRight);
        }

        glm::vec2 get_trigger_stick(int player_index, const AxixConfig& config) const {
            return get_normalized_stick(player_index, SDL_GAMEPAD_AXIS_LEFT_TRIGGER, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
                                        config.deadZoneTrigger);
        }

      private:
        // Processa Deadzone Radial combinando eixos X e Y usando GLM
        glm::vec2 get_normalized_stick(int player_index, SDL_GamepadAxis axis_x, SDL_GamepadAxis axis_y,
                                       float dead_zone) const {

            if (!player_mappings_.contains(player_index)) {
                return glm::vec2(0.0F);
            }

            glm::vec2 combined_input(0.0F);

            for (SDL_JoystickID id : player_mappings_.at(player_index)) {
                if (!connected_gamepads_.contains(id)) {
                    continue;
                }

                SDL_Gamepad* gamepad = connected_gamepads_.at(id);

                // Valores brutos do SDL3 vão de -32768 a 32767
                float raw_x = static_cast<float>(SDL_GetGamepadAxis(gamepad, axis_x)) / 32767.0F;
                float raw_y = static_cast<float>(SDL_GetGamepadAxis(gamepad, axis_y)) / 32767.0F;

                glm::vec2 input(raw_x, raw_y);
                float length = glm::length(input);

                if (length > dead_zone) {
                    // Normaliza o vetor após a deadzone para não perder precisão linear inicial
                    const glm::vec2 dir = input / length;
                    const float normalized_length = (length - dead_zone) / (1.0F - dead_zone);
                    combined_input += dir * glm::clamp(normalized_length, 0.0F, 1.0F);
                }
            }

            // Limita o output combinado caso o jogador mova sticks de dois controles físicos ao mesmo tempo
            if (glm::length(combined_input) > 1.0F) {
                combined_input = glm::normalize(combined_input);
            }

            return combined_input;
        }

        std::unordered_map<SDL_JoystickID, SDL_Gamepad*> connected_gamepads_;
        std::unordered_map<int, std::vector<SDL_JoystickID>> player_mappings_;
        std::unordered_map<SDL_JoystickID, std::unordered_map<SDL_GamepadButton, ButtonState>> button_states_;
    };
} // namespace ce
