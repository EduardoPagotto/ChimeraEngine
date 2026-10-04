#pragma once

#include "GamePad.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "event.hpp"
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
        InputManager() {
            this->keyboard_ = std::make_shared<Keyboard>();
            this->mouse_ = std::make_shared<Mouse>();
            this->gamepad_ = std::make_shared<Gamepad>();
        };

        virtual ~InputManager() {
            this->keyboard_.reset();
            this->mouse_.reset();
            this->gamepad_.reset();
        }

        // Limpa os gatilhos rápidos do frame anterior. Chame no INÍCIO do loop principal.
        void startFrame() {
            this->keyboard_->startFrame();
            this->mouse_->startFrame();
        }

        // Processa os eventos de clique único vindos do SDL_PollEvent (Garante 100% de detecção)
        bool handleEvent(const SDL_Event& event) {

            bool doneHere = this->keyboard_->handleEvent(event);
            if (!doneHere) {
                doneHere = this->mouse_->handleEvent(event);
            }

            if (!doneHere) {
                doneHere = this->gamepad_->handleEvent(event);
            }

            if (!doneHere && event.type == chimera_even_t01) {
                auto c = static_cast<EventCE>(event.user.code);
                if (c == EventCE::FLOW_PAUSE) {

                    this->paused_ = true;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                    doneHere = true;

                } else if (c == EventCE::FLOW_RESUME) {

                    this->paused_ = false;
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
            this->keyboard_->updateContinuousInput();
            this->mouse_->updateContinuousInput();
            this->gamepad_->updateContinuousInput();
        }

        std::shared_ptr<Keyboard> getKeyboard() { return this->keyboard_; }
        std::shared_ptr<Mouse> getMouse() { return this->mouse_; }
        std::shared_ptr<Gamepad> getGamepad() { return this->gamepad_; }

        bool getStatusPause() const { return this->paused_; }

      private:
        bool paused_{false};
        std::shared_ptr<Mouse> mouse_;
        std::shared_ptr<Keyboard> keyboard_;
        std::shared_ptr<Gamepad> gamepad_;
    };
} // namespace ce
