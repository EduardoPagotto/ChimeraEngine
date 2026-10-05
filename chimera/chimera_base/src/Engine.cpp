#include "chimera_base/Engine.hpp"
#include "chimera_base/InputManager.hpp"
#include "chimera_base/event.hpp"

namespace ce {

    Engine::Engine(std::shared_ptr<entt::registry> registry) : registry_(registry) {

        canva_ = registry->ctx().get<std::shared_ptr<ICanva>>();

        timer_fps_.set_elapsed_count(1000);
        timer_fps_.start();
        chimera_even_t01 = SDL_RegisterEvents(1);

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Engine Chimera OK");
    }

    void Engine::run() { // NOLINT

        auto& im = registry_->ctx().get<std::shared_ptr<InputManager>>();

        SDL_Event event;
        bool kill{false};
        uint32_t begin_count{0};
        uint32_t count_delta{7};
        double ts{0.0F};

        while (!kill) {

            begin_count = SDL_GetTicks();

            im->start_frame();

            while (SDL_PollEvent(&event)) {

                switch (event.type) {
                    // Windows
                    case SDL_EVENT_WINDOW_RESIZED: {
                        const int32_t nova_width = event.window.data1;
                        const int32_t nova_height = event.window.data2;
                        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Resize screem received: %d x %d", nova_width,
                                     nova_height);
                        canva_->reshape(nova_width, nova_height);

                    } break;
                    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                        const int new_width = event.window.data1;
                        const int new_height = event.window.data2;

                        SDL_LogDebug(SDL_LOG_CATEGORY_VIDEO, "Pixel change (%d x %d)", new_width, new_height);

                    } break;
                    case SDL_EVENT_WINDOW_MAXIMIZED:
                    case SDL_EVENT_WINDOW_RESTORED: {
                        send_chimera_event(EventCE::FLOW_RESUME, nullptr, nullptr);
                        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Windows restored/maximized");

                    } break;
                    case SDL_EVENT_WINDOW_MINIMIZED: {
                        send_chimera_event(EventCE::FLOW_PAUSE, nullptr, nullptr);
                        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Windows minimized");

                    } break;
                    case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
                        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Fullscreem ON");
                        break;

                    case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
                        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Fullscreem OFF");
                        break;

                    case SDL_EVENT_QUIT:
                        kill = true;
                        break;

                    default:
                        if (event.type == chimera_even_t01) {
                            if (static_cast<EventCE>(event.user.code) == EventCE::TOGGLE_FULL_SCREEN) {
                                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Toggle fullscreem received");
                                canva_->toggle_fullscreen();
                            }
                        }
                        break;
                }

                im->handle_event(event);

                for (auto& ev : stack_) {
                    ev->on_event(event);
                }
            }

            // Atualiza o estado das teclas que continuam pressionadas
            im->update_continuous_input();

            ts = (double)count_delta / 1000.0F;
            if (!im->get_status_pause()) { // update game

                for (auto iten : stack_) {
                    iten->on_update(ts);
                }

                canva_->before();

                for (auto iten : stack_) {
                    iten->on_render();
                }

                canva_->after();
            }

            if (timer_fps_.step_count()) { // count FPS each second
                fps_ = timer_fps_.get_count_step();
                send_chimera_event(EventCE::NEW_FPS, (void*)&fps_, nullptr);
            }

            count_delta = SDL_GetTicks() - begin_count; // frame count limit
            if (count_delta < minium_count_delta) {
                SDL_Delay(minium_count_delta - count_delta);
                count_delta = minium_count_delta;
            }
        }
    }
} // namespace ce
