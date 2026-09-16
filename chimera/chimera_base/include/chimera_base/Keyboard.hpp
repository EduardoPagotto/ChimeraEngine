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
            int numKeys = 0;
            SDL_GetKeyboardState(&numKeys);

            this->keyboardState.resize(numKeys, 0);
            this->pressedKeys.resize(numKeys, 0);
            this->releasedKeys.resize(numKeys, 0);
        }

        // Limpa os gatilhos rápidos do frame anterior. Chame no INÍCIO do loop principal.
        void startFrame() {
            // teclado
            std::fill(this->pressedKeys.begin(), this->pressedKeys.end(), 0);
            std::fill(this->releasedKeys.begin(), this->releasedKeys.end(), 0);
        }

        // Processa os eventos de clique único vindos do SDL_PollEvent (Garante 100% de detecção)
        bool handleEvent(const SDL_Event& event) {

            bool doneHere = true;
            switch (event.type) {
                case SDL_EVENT_KEY_DOWN: {
                    size_t scancode = static_cast<size_t>(event.key.scancode);
                    if (scancode < this->pressedKeys.size() && !event.key.repeat) {
                        this->pressedKeys[scancode] = 1; // Registra o exato momento do clique
                    }
                } break;
                case SDL_EVENT_KEY_UP: {
                    size_t scancode = static_cast<size_t>(event.key.scancode);
                    if (scancode < this->releasedKeys.size()) {
                        this->releasedKeys[scancode] = 1; // Registra o exato momento em que soltou
                    }
                } break;
                default:
                    doneHere = false;
                    break;
            }

            return doneHere;
        }

        // Atualiza o estado contínuo do teclado (Para movimentação simultânea sem delay)
        void updateContinuousInput() {
            int numKeys = 0;
            const bool* keyboardState = SDL_GetKeyboardState(&numKeys);

            if ((keyboardState != nullptr) && numKeys > 0) {
                std::span<const bool> stateSpan(keyboardState, numKeys);
                std::copy(stateSpan.begin(), stateSpan.end(), this->keyboardState.begin());
            }
        }

        // [CONTINUAMENTE PRESSIONADA]: Perfeito para andar/correr com múltiplas teclas ao mesmo tempo
        [[nodiscard]] bool isKeyDown(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= this->keyboardState.size()) {
                return false;
            }

            return this->keyboardState[static_cast<size_t>(scancode)] != 0;
        }

        // [PRESSIONADA NESTE FRAME]: Pega cliques instantâneos, sem falhas (Pular, Atirar, Abrir Menu)
        [[nodiscard]] bool isKeyPressed(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= this->pressedKeys.size()) {
                return false;
            }

            return this->pressedKeys[static_cast<size_t>(scancode)] != 0;
        }

        // [LIBERADA NESTE FRAME]: Detecta o momento exato em que a tecla foi solta
        [[nodiscard]] bool isKeyReleased(SDL_Scancode scancode) const {
            if (static_cast<size_t>(scancode) >= this->releasedKeys.size()) {
                return false;
            }

            return this->releasedKeys[static_cast<size_t>(scancode)] != 0;
        }

      private:
        std::vector<uint8_t> keyboardState;
        std::vector<uint8_t> pressedKeys;
        std::vector<uint8_t> releasedKeys;
    };
} // namespace ce
