#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <cstdint>
#include <glm/glm.hpp>

namespace ce {

    /// @brief Mouse Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20261004
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
            this->continuous_buttons_.fill(false);
            this->pressed_buttons_.fill(false);
            this->released_buttons_.fill(false);
        }

        virtual ~Mouse() = default;

        // Limpa os gatilhos rápidos e os deltas acumulados no frame anterior
        void start_frame() {
            this->pressed_buttons_.fill(false);
            this->released_buttons_.fill(false);
            this->delta_ = {0.0F, 0.0F};
            this->scrolll_ = {0.0F, 0.0F};
        }

        // Captura os eventos de hardware brutos do SDL_PollEvent (Garante zero latência e perda)
        bool handle_event(const SDL_Event& event) noexcept {

            bool done_here = true;
            switch (event.type) {
                case SDL_EVENT_MOUSE_MOTION:
                    // No SDL3, coordenadas e deltas de movimento usam floats
                    position_ = {event.motion.x, event.motion.y};
                    // Acumula caso ocorram múltiplos sub-frames
                    delta_ += glm::vec2(event.motion.xrel, event.motion.yrel);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    uint8_t button_index = event.button.button - 1;
                    if (button_index < TotalButtons) {
                        this->pressed_buttons_[button_index] = true;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    uint8_t button_index = event.button.button - 1;
                    if (button_index < TotalButtons) {
                        this->released_buttons_[button_index] = true;
                    }
                    break;
                }

                case SDL_EVENT_MOUSE_WHEEL:
                    // SDL3 usa floats para o roller para suportar scrolls de precisão livre
                    this->scrolll_ += glm::vec2(event.wheel.x, event.wheel.y);
                    // m_scrollY += event.wheel.y; // Geralmente o scroll vertical padrão
                    break;
                default:
                    done_here = false;
                    break;
            }

            return done_here;
        }

        // Atualiza o estado contínuo (se botões continuam apertados)
        void update_continuous_input() {
            uint32_t button_mask = SDL_GetMouseState(nullptr, nullptr);
            this->continuous_buttons_[Left] = (button_mask & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) != 0;
            this->continuous_buttons_[Middle] = (button_mask & SDL_BUTTON_MASK(SDL_BUTTON_MIDDLE)) != 0;
            this->continuous_buttons_[Right] = (button_mask & SDL_BUTTON_MASK(SDL_BUTTON_RIGHT)) != 0;
        }

        // Retona posicao do mouse
        [[nodiscard]] glm::vec2 get_position_xy() const { return this->position_; }

        // Retorna o deslocamento (Delta) ocorrido neste frame
        [[nodiscard]] glm::vec2 get_delta_xy() const { return this->delta_; }

        // Retorna o scroll da rodinha ocorrido neste frame (Positivo = Cima/Direita, Negativo = Baixo/Esquerda)
        [[nodiscard]] glm::vec2 get_scroll() const { return this->scrolll_; }

        [[nodiscard]] bool is_button_down(MouseButton button) const { return this->continuous_buttons_[button]; }
        [[nodiscard]] bool is_button_key_pressed(MouseButton button) const { return this->pressed_buttons_[button]; }
        [[nodiscard]] bool is_button_key_released(MouseButton button) const { return this->released_buttons_[button]; }

      private:
        glm::vec2 position_{0.0F, 0.0F}; // float m_mouseX, m_mouseY;
        glm::vec2 delta_{0.0F, 0.0F};    // float m_deltaX, m_deltaY;
        glm::vec2 scrolll_{0.0F, 0.0F};  //  float m_scrollX, m_scrollY;

        std::array<bool, TotalButtons> continuous_buttons_{};
        std::array<bool, TotalButtons> pressed_buttons_{};
        std::array<bool, TotalButtons> released_buttons_{};
    };
} // namespace ce
