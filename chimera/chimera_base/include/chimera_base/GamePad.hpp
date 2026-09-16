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
    /// @date 20260915
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
            for (auto& [id, gamepad] : connectedGamepads) {
                SDL_CloseGamepad(gamepad);
            }
            connectedGamepads.clear();
        }

        // Associa um Gamepad físico (via Instance ID) a um Jogador lógico
        void assignGamepadToPlayer(int playerIndex, SDL_JoystickID instanceId) {
            playerMappings[playerIndex].push_back(instanceId);
        }

        // Atualiza os estados físicos - Chame uma vez no início do seu Game Loop
        bool handleEvent(const SDL_Event& event) noexcept { // NOLINT

            if (event.type == SDL_EVENT_GAMEPAD_ADDED) {

                SDL_Gamepad* gamepad = SDL_OpenGamepad(event.gdevice.which);
                if (gamepad != nullptr) {
                    SDL_JoystickID id = SDL_GetGamepadID(gamepad);
                    connectedGamepads[id] = gamepad;
                    // Por padrão, joga novos controles para o Player 0 (Customizável)
                    assignGamepadToPlayer(0, id);

                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Gamepad conectado ID: %d", id);
                    GetInfoPad(id);
                }

            } else if (event.type == SDL_EVENT_GAMEPAD_REMOVED) {

                SDL_JoystickID id = event.gdevice.which;
                if (connectedGamepads.contains(id)) {
                    SDL_CloseGamepad(connectedGamepads[id]);
                    connectedGamepads.erase(id);
                    // Remove mapeamento do jogador
                    for (auto& [player, list] : playerMappings) {
                        std::erase(list, id);
                    }
                    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Gamepad desconectado ID: %d", id);
                }
            } else {
                return false;
            }

            return true;
        }

        static void GetInfoPad(const SDL_JoystickID& instance_id) {

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

        void updateContinuousInput() {

            for (auto& [id, gamepad] : connectedGamepads) {

                auto& bStates = buttonStates[id];

                for (int b = SDL_GAMEPAD_BUTTON_SOUTH; b < SDL_GAMEPAD_BUTTON_COUNT; ++b) {

                    auto button = static_cast<SDL_GamepadButton>(b);
                    bool isDown = SDL_GetGamepadButton(gamepad, button);

                    ButtonState& current = bStates[button];

                    if (isDown) {

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

        ButtonState getButtonState(int playerIndex, SDL_GamepadButton button) const {
            if (!playerMappings.contains(playerIndex)) {
                return ButtonState::None;
            }

            bool anyPressed = false;
            bool anyHeld = false;
            bool anyReleased = false;

            for (SDL_JoystickID id : playerMappings.at(playerIndex)) {
                if (!buttonStates.contains(id) || !buttonStates.at(id).contains(button)) {
                    continue;
                }

                ButtonState state = buttonStates.at(id).at(button);
                if (state == ButtonState::Pressed) {
                    anyPressed = true;
                }
                if (state == ButtonState::Held) {
                    anyHeld = true;
                }
                if (state == ButtonState::Released) {
                    anyReleased = true;
                }
            }

            if (anyPressed) {
                return ButtonState::Pressed;
            }

            if (anyHeld) {
                return ButtonState::Held;
            }

            if (anyReleased) {
                return ButtonState::Released;
            }

            return ButtonState::None;
        }

        glm::vec2 getLeftStick(int playerIndex, const AxixConfig& config) const {
            return getNormalizedStick(playerIndex, SDL_GAMEPAD_AXIS_LEFTX, SDL_GAMEPAD_AXIS_LEFTY, config.deadZoneLeft);
        }

        glm::vec2 getRightStick(int playerIndex, const AxixConfig& config) const {
            return getNormalizedStick(playerIndex, SDL_GAMEPAD_AXIS_RIGHTX, SDL_GAMEPAD_AXIS_RIGHTY,
                                      config.deadZoneRight);
        }

        glm::vec2 getTriggerStick(int playerIndex, const AxixConfig& config) const {
            return getNormalizedStick(playerIndex, SDL_GAMEPAD_AXIS_LEFT_TRIGGER, SDL_GAMEPAD_AXIS_RIGHT_TRIGGER,
                                      config.deadZoneTrigger);
        }

      private:
        // Processa Deadzone Radial combinando eixos X e Y usando GLM
        glm::vec2 getNormalizedStick(int playerIndex, SDL_GamepadAxis axisX, SDL_GamepadAxis axisY,
                                     float deadZone) const {

            if (!playerMappings.contains(playerIndex)) {
                return glm::vec2(0.0F);
            }

            glm::vec2 combinedInput(0.0F);

            for (SDL_JoystickID id : playerMappings.at(playerIndex)) {
                if (!connectedGamepads.contains(id)) {
                    continue;
                }

                SDL_Gamepad* gamepad = connectedGamepads.at(id);

                // Valores brutos do SDL3 vão de -32768 a 32767
                float rawX = static_cast<float>(SDL_GetGamepadAxis(gamepad, axisX)) / 32767.0F;
                float rawY = static_cast<float>(SDL_GetGamepadAxis(gamepad, axisY)) / 32767.0F;

                glm::vec2 input(rawX, rawY);
                float length = glm::length(input);

                if (length > deadZone) {
                    // Normaliza o vetor após a deadzone para não perder precisão linear inicial
                    glm::vec2 dir = input / length;
                    float normalizedLength = (length - deadZone) / (1.0F - deadZone);
                    combinedInput += dir * glm::clamp(normalizedLength, 0.0F, 1.0F);
                }
            }

            // Limita o output combinado caso o jogador mova sticks de dois controles físicos ao mesmo tempo
            if (glm::length(combinedInput) > 1.0F) {
                combinedInput = glm::normalize(combinedInput);
            }

            return combinedInput;
        }

        std::unordered_map<SDL_JoystickID, SDL_Gamepad*> connectedGamepads;
        std::unordered_map<int, std::vector<SDL_JoystickID>> playerMappings; // Player ID -> Lista de controles físicos
        std::unordered_map<SDL_JoystickID, std::unordered_map<SDL_GamepadButton, ButtonState>> buttonStates;
    };
} // namespace ce
