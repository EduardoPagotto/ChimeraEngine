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
    /// @date 20261004
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
        void start_frame() {
            this->keyboard_->start_frame();
            this->mouse_->start_frame();
        }

        // Processa os eventos de clique único vindos do SDL_PollEvent (Garante 100% de detecção)
        bool handle_event(const SDL_Event& event) {

            bool done_here = this->keyboard_->handle_event(event);
            if (!done_here) {
                done_here = this->mouse_->handle_event(event);
            }

            if (!done_here) {
                done_here = this->gamepad_->handleEvent(event);
            }

            if (!done_here && event.type == chimera_even_t01) {
                auto c = static_cast<EventCE>(event.user.code);
                if (c == EventCE::FLOW_PAUSE) {

                    this->paused_ = true;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                    done_here = true;

                } else if (c == EventCE::FLOW_RESUME) {

                    this->paused_ = false;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
                    done_here = true;

                } else if (c == EventCE::FLOW_STOP) {

                    SDL_Event l_event_quit;
                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "QUIT Receive");
                    l_event_quit.type = SDL_EVENT_QUIT;
                    if (!SDL_PushEvent(&l_event_quit)) {
                        throw std::runtime_error(std::format("Critical SDL_QUIT PushEvent fail: {}", SDL_GetError()));
                    }
                }

                done_here = true;
            }

            return done_here;
        }

        // Atualiza o estado contínuo do teclado (Para movimentação simultânea sem delay)
        void update_continuous_input() {
            this->keyboard_->update_continuous_input();
            this->mouse_->update_continuous_input();
            this->gamepad_->updateContinuousInput();
        }

        std::shared_ptr<Keyboard> get_keyboard() { return this->keyboard_; }
        std::shared_ptr<Mouse> get_mouse() { return this->mouse_; }
        std::shared_ptr<Gamepad> get_gamepad() { return this->gamepad_; }

        bool get_status_pause() const { return this->paused_; }

      private:
        bool paused_{false};
        std::shared_ptr<Mouse> mouse_;
        std::shared_ptr<Keyboard> keyboard_;
        std::shared_ptr<Gamepad> gamepad_;
    };
} // namespace ce
