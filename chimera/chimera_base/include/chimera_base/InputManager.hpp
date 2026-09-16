#pragma once

#include <SDL3/SDL.h>
#include <algorithm>
#include <cstdint>
#include <span>
#include <vector>

namespace ce {

    class InputManager {
      public:
        enum MouseButton : uint8_t {
            Left = SDL_BUTTON_LEFT - 1,
            Middle = SDL_BUTTON_MIDDLE - 1,
            Right = SDL_BUTTON_RIGHT - 1,
            TotalButtons = 3
        };

        InputManager()
            : m_mouseX(0.0f), m_mouseY(0.0f), m_deltaX(0.0f), m_deltaY(0.0f), m_scrollX(0.0f), m_scrollY(0.0f) {

            // teclado
            int numKeys = 0;
            SDL_GetKeyboardState(&numKeys);

            m_keyboardState.resize(numKeys, 0);
            m_pressedKeys.resize(numKeys, 0);
            m_releasedKeys.resize(numKeys, 0);

            // mouse
            m_continuousButtons.fill(false);
            m_pressedButtons.fill(false);
            m_releasedButtons.fill(false);
        }

        // Limpa os gatilhos rápidos do frame anterior. Chame no INÍCIO do loop principal.
        void startFrame() {
            // teclado
            std::fill(m_pressedKeys.begin(), m_pressedKeys.end(), 0);
            std::fill(m_releasedKeys.begin(), m_releasedKeys.end(), 0);

            // mouse
            m_pressedButtons.fill(false);
            m_releasedButtons.fill(false);
            m_deltaX = 0.0f;
            m_deltaY = 0.0f;
            m_scrollX = 0.0f;
            m_scrollY = 0.0f;
        }

        // Processa os eventos de clique único vindos do SDL_PollEvent (Garante 100% de detecção)
        void handleEvent(const SDL_Event& event) {
            // if (event.type == SDL_EVENT_KEY_DOWN) {
            //     size_t scancode = static_cast<size_t>(event.key.scancode);
            //     if (scancode < m_pressedKeys.size() && !event.key.repeat) {
            //         m_pressedKeys[scancode] = 1; // Registra o exato momento do clique
            //     }
            // } else if (event.type == SDL_EVENT_KEY_UP) {
            //     size_t scancode = static_cast<size_t>(event.key.scancode);
            //     if (scancode < m_releasedKeys.size()) {
            //         m_releasedKeys[scancode] = 1; // Registra o exato momento em que soltou
            //     }
            // }

            // mouse
            switch (event.type) {
                case SDL_EVENT_KEY_DOWN: {
                    size_t scancode = static_cast<size_t>(event.key.scancode);
                    if (scancode < m_pressedKeys.size() && !event.key.repeat) {
                        m_pressedKeys[scancode] = 1; // Registra o exato momento do clique
                    }
                } break;
                case SDL_EVENT_KEY_UP: {
                    size_t scancode = static_cast<size_t>(event.key.scancode);
                    if (scancode < m_releasedKeys.size()) {
                        m_releasedKeys[scancode] = 1; // Registra o exato momento em que soltou
                    }
                } break;
                case SDL_EVENT_MOUSE_MOTION:
                    // No SDL3, coordenadas e deltas de movimento usam floats
                    m_mouseX = event.motion.x;
                    m_mouseY = event.motion.y;
                    m_deltaX += event.motion.xrel; // Acumula caso ocorram múltiplos sub-frames
                    m_deltaY += event.motion.yrel;
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    uint8_t buttonIndex = event.button.button - 1;
                    if (buttonIndex < TotalButtons) {
                        m_pressedButtons[buttonIndex] = true;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    uint8_t buttonIndex = event.button.button - 1;
                    if (buttonIndex < TotalButtons) {
                        m_releasedButtons[buttonIndex] = true;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_WHEEL:
                    // SDL3 usa floats para o roller para suportar scrolls de precisão livre
                    m_scrollX += event.wheel.x;
                    m_scrollY += event.wheel.y; // Geralmente o scroll vertical padrão
                    break;
                default:
                    break;
            }
        }

        // Atualiza o estado contínuo do teclado (Para movimentação simultânea sem delay)
        void updateContinuousInput() {
            int numKeys = 0;
            const bool* keyboardState = SDL_GetKeyboardState(&numKeys);

            if ((keyboardState != nullptr) && numKeys > 0) {
                std::span<const bool> stateSpan(keyboardState, numKeys);
                std::copy(stateSpan.begin(), stateSpan.end(), m_keyboardState.begin());
            }

            uint32_t buttonMask = SDL_GetMouseState(nullptr, nullptr);
            m_continuousButtons[Left] = (buttonMask & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
            m_continuousButtons[Middle] = (buttonMask & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) != 0;
            m_continuousButtons[Right] = (buttonMask & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0;
        }

        // [CONTINUAMENTE PRESSIONADA]: Perfeito para andar/correr com múltiplas teclas ao mesmo tempo
        [[nodiscard]] bool isKeyDown(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= m_keyboardState.size()) {
                return false;
            }

            return m_keyboardState[static_cast<size_t>(scancode)] != 0;
        }

        // [PRESSIONADA NESTE FRAME]: Pega cliques instantâneos, sem falhas (Pular, Atirar, Abrir Menu)
        [[nodiscard]] bool isKeyPressed(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= m_pressedKeys.size()) {
                return false;
            }

            return m_pressedKeys[static_cast<size_t>(scancode)] != 0;
        }

        // [LIBERADA NESTE FRAME]: Detecta o momento exato em que a tecla foi solta
        [[nodiscard]] bool isKeyReleased(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= m_releasedKeys.size()) {
                return false;
            }

            return m_releasedKeys[static_cast<size_t>(scancode)] != 0;
        }

        bool getStatusPause() const { return this->paused; }
        void setStatusPause(const bool& val) { this->paused = val; }

        // --- GETTERS ---
        [[nodiscard]] float getMouseX() const { return m_mouseX; }
        [[nodiscard]] float getMouseY() const { return m_mouseY; }

        // Retorna o deslocamento (Delta) ocorrido neste frame
        [[nodiscard]] float getDeltaX() const { return m_deltaX; }
        [[nodiscard]] float getDeltaY() const { return m_deltaY; }

        // Retorna o scroll da rodinha ocorrido neste frame (Positivo = Cima/Direita, Negativo = Baixo/Esquerda)
        [[nodiscard]] float getScrollX() const { return m_scrollX; }
        [[nodiscard]] float getScrollY() const { return m_scrollY; }

        [[nodiscard]] bool isButtonDown(MouseButton button) const { return m_continuousButtons[button]; }
        [[nodiscard]] bool isButtonKeyPressed(MouseButton button) const { return m_pressedButtons[button]; }
        [[nodiscard]] bool isButtonKeyReleased(MouseButton button) const { return m_releasedButtons[button]; }

      private:
        bool paused{false};
        std::vector<uint8_t> m_keyboardState;
        std::vector<uint8_t> m_pressedKeys;
        std::vector<uint8_t> m_releasedKeys;
        // Nouse
        float m_mouseX, m_mouseY;
        float m_deltaX, m_deltaY;
        float m_scrollX, m_scrollY;
        std::array<bool, TotalButtons> m_continuousButtons{};
        std::array<bool, TotalButtons> m_pressedButtons{};
        std::array<bool, TotalButtons> m_releasedButtons{};
    };
} // namespace ce
