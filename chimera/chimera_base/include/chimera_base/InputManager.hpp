#pragma once

#include "chimera_base/GamePad.hpp"
#include "chimera_base/Keyboard.hpp"
#include "chimera_base/Mouse.hpp"
#include "chimera_base/event.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>

namespace ce {

    /// @brief Input central
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20260915
    /// @date 20260910
    class InputManager {
      public:
        explicit InputManager() {
            this->keyboard = std::make_shared<Keyboard>();
            this->mouse = std::make_shared<Mouse>();
            this->gamepad = std::make_shared<Gamepad>();
        };

        virtual ~InputManager() {
            this->keyboard.reset();
            this->mouse.reset();
            this->gamepad.reset();
        }

        // Limpa os gatilhos rápidos do frame anterior. Chame no INÍCIO do loop principal.
        void startFrame() {
            this->keyboard->startFrame();
            this->mouse->startFrame();
        }

        // Processa os eventos de clique único vindos do SDL_PollEvent (Garante 100% de detecção)
        bool handleEvent(const SDL_Event& event) {

            bool doneHere = this->keyboard->handleEvent(event);
            if (!doneHere) {
                doneHere = this->mouse->handleEvent(event);
            }

            if (!doneHere) {
                doneHere = this->gamepad->handleEvent(event);
            }

            if (!doneHere && event.type == CHIMERA_EVENT01) {
                auto c = static_cast<EventCE>(event.user.code);
                if (c == EventCE::FLOW_PAUSE) {

                    this->paused = true;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                    doneHere = true;

                } else if (c == EventCE::FLOW_RESUME) {

                    this->paused = false;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
                    doneHere = true;

                } else if (c == EventCE::FLOW_STOP) {

                    SDL_Event l_eventQuit;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "QUIT Receive");
                    l_eventQuit.type = SDL_EVENT_QUIT;
                    if (!SDL_PushEvent(&l_eventQuit)) {
                        throw std::runtime_error(std::format("Critical SDL_QUIT PushEvent fail: {}", SDL_GetError()));
                    }
                }

                doneHere = true;
            }

            return doneHere;
        }

        // Atualiza o estado contínuo do teclado (Para movimentação simultânea sem delay)
        void updateContinuousInput() {
            this->keyboard->updateContinuousInput();
            this->mouse->updateContinuousInput();
            this->gamepad->updateContinuousInput();
        }

        std::shared_ptr<Keyboard> getKeyboard() { return this->keyboard; }
        std::shared_ptr<Mouse> getMouse() { return this->mouse; }
        std::shared_ptr<Gamepad> getGamepad() { return this->gamepad; }

        bool getStatusPause() const { return this->paused; }

      private:
        bool paused{false};
        std::shared_ptr<Mouse> mouse;
        std::shared_ptr<Keyboard> keyboard;
        std::shared_ptr<Gamepad> gamepad;
    };
} // namespace ce
