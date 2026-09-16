#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>

namespace ce {

    /// @brief Mouse Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260915
    class Mouse {
        // friend struct InputManager;
      public:
        enum MouseButton : uint8_t {
            Left = SDL_BUTTON_LEFT - 1,
            Middle = SDL_BUTTON_MIDDLE - 1,
            Right = SDL_BUTTON_RIGHT - 1,
            TotalButtons = 3
        };

        Mouse() {
            this->continuousButtons.fill(false);
            this->pressedButtons.fill(false);
            this->releasedButtons.fill(false);
        }

        virtual ~Mouse() = default;

        // Limpa os gatilhos rápidos e os deltas acumulados no frame anterior
        void startFrame() {
            this->pressedButtons.fill(false);
            this->releasedButtons.fill(false);
            this->delta = {0.0F, 0.0F};
            this->scrolll = {0.0F, 0.0F};
        }

        // Captura os eventos de hardware brutos do SDL_PollEvent (Garante zero latência e perda)
        bool handleEvent(const SDL_Event& event) noexcept {

            bool doneHere = true;
            switch (event.type) {
                case SDL_EVENT_MOUSE_MOTION:
                    // No SDL3, coordenadas e deltas de movimento usam floats
                    position = {event.motion.x, event.motion.y};
                    // Acumula caso ocorram múltiplos sub-frames
                    delta += glm::vec2(event.motion.xrel, event.motion.yrel);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    uint8_t buttonIndex = event.button.button - 1;
                    if (buttonIndex < TotalButtons) {
                        this->pressedButtons[buttonIndex] = true;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    uint8_t buttonIndex = event.button.button - 1;
                    if (buttonIndex < TotalButtons) {
                        this->releasedButtons[buttonIndex] = true;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_WHEEL:
                    // SDL3 usa floats para o roller para suportar scrolls de precisão livre
                    this->scrolll += glm::vec2(event.wheel.x, event.wheel.y);
                    // m_scrollY += event.wheel.y; // Geralmente o scroll vertical padrão
                    break;
                default:
                    doneHere = false;
                    break;
            }

            return doneHere;
        }

        // Atualiza o estado contínuo (se botões continuam apertados)
        void updateContinuousInput() {
            uint32_t buttonMask = SDL_GetMouseState(nullptr, nullptr);
            this->continuousButtons[Left] = (buttonMask & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
            this->continuousButtons[Middle] = (buttonMask & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) != 0;
            this->continuousButtons[Right] = (buttonMask & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0;
        }

        // Retona posicao do mouse
        [[nodiscard]] glm::vec2 getPositionXY() const { return this->position; }

        // Retorna o deslocamento (Delta) ocorrido neste frame
        [[nodiscard]] glm::vec2 getDeltaXY() const { return this->delta; }

        // Retorna o scroll da rodinha ocorrido neste frame (Positivo = Cima/Direita, Negativo = Baixo/Esquerda)
        [[nodiscard]] glm::vec2 getScroll() const { return this->scrolll; }

        [[nodiscard]] bool isButtonDown(MouseButton button) const { return this->continuousButtons[button]; }
        [[nodiscard]] bool isButtonKeyPressed(MouseButton button) const { return this->pressedButtons[button]; }
        [[nodiscard]] bool isButtonKeyReleased(MouseButton button) const { return this->releasedButtons[button]; }

      private:
        glm::vec2 position{0.0F, 0.0F}; // float m_mouseX, m_mouseY;
        glm::vec2 delta{0.0F, 0.0F};    // float m_deltaX, m_deltaY;
        glm::vec2 scrolll{0.0F, 0.0F};  //  float m_scrollX, m_scrollY;

        std::array<bool, TotalButtons> continuousButtons{};
        std::array<bool, TotalButtons> pressedButtons{};
        std::array<bool, TotalButtons> releasedButtons{};
    };
} // namespace ce
