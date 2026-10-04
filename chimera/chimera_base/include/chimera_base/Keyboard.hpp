#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
#include <cstdint>
#include <span>
#include <vector>

namespace ce {

    /// @brief Keyboard Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260915
    class Keyboard {
      public:
        Keyboard() {
            int num_keys = 0;
            SDL_GetKeyboardState(&num_keys);

            this->keyboard_state_.resize(num_keys, 0);
            this->pressed_keys_.resize(num_keys, 0);
            this->released_keys_.resize(num_keys, 0);
        }

        // Limpa os gatilhos rápidos do frame anterior. Chame no INÍCIO do loop principal.
        void start_frame() {
            // teclado
            std::fill(this->pressed_keys_.begin(), this->pressed_keys_.end(), 0);
            std::fill(this->released_keys_.begin(), this->released_keys_.end(), 0);
        }

        // Processa os eventos de clique único vindos do SDL_PollEvent (Garante 100% de detecção)
        bool handle_event(const SDL_Event& event) {

            bool done_here = true;
            switch (event.type) {
                case SDL_EVENT_KEY_DOWN: {
                    size_t scancode = static_cast<size_t>(event.key.scancode);
                    if (scancode < this->pressed_keys_.size() && !event.key.repeat) {
                        this->pressed_keys_[scancode] = 1; // Registra o exato momento do clique
                    }
                } break;
                case SDL_EVENT_KEY_UP: {
                    size_t scancode = static_cast<size_t>(event.key.scancode);
                    if (scancode < this->released_keys_.size()) {
                        this->released_keys_[scancode] = 1; // Registra o exato momento em que soltou
                    }
                } break;
                default:
                    done_here = false;
                    break;
            }

            return done_here;
        }

        // Atualiza o estado contínuo do teclado (Para movimentação simultânea sem delay)
        void update_continuous_input() {
            int num_keys = 0;
            const bool* keyboardState = SDL_GetKeyboardState(&num_keys);

            if ((keyboardState != nullptr) && num_keys > 0) {
                std::span<const bool> state_span(keyboardState, num_keys);
                std::copy(state_span.begin(), state_span.end(), this->keyboard_state_.begin());
            }
        }

        // [CONTINUAMENTE PRESSIONADA]: Perfeito para andar/correr com múltiplas teclas ao mesmo tempo
        [[nodiscard]] bool is_key_down(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= this->keyboard_state_.size()) {
                return false;
            }

            return this->keyboard_state_[static_cast<size_t>(scancode)] != 0;
        }

        // [PRESSIONADA NESTE FRAME]: Pega cliques instantâneos, sem falhas (Pular, Atirar, Abrir Menu)
        [[nodiscard]] bool is_key_pressed(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= this->pressed_keys_.size()) {
                return false;
            }

            return this->pressed_keys_[static_cast<size_t>(scancode)] != 0;
        }

        // [LIBERADA NESTE FRAME]: Detecta o momento exato em que a tecla foi solta
        [[nodiscard]] bool is_key_released(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= this->released_keys_.size()) {
                return false;
            }

            return this->released_keys_[static_cast<size_t>(scancode)] != 0;
        }

      private:
        std::vector<uint8_t> keyboard_state_;
        std::vector<uint8_t> pressed_keys_;
        std::vector<uint8_t> released_keys_;
    };
} // namespace ce
