#pragma once
#include "ServiceLocator.hpp"
#include "StateStack.hpp"
#include "Timer.hpp"
#include "event.hpp"
#include "interfaces.hpp"

namespace ce {

    constexpr uint32_t miniumCountDelta = 1000 / 140;

    /// @brief Engine
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20250401
    class Engine {

      private:
        std::shared_ptr<ICanva> canvas;
        uint32_t fps = 140;
        Timer timerFPS;
        StateStack stack;

      public:
        Engine() {

            timerFPS.setElapsedCount(1000);
            timerFPS.start();

            canvas = g_service_locator.getService<ICanva>();
            SDL_Log("Engine Register: chimera_engine OK");
        }

        virtual ~Engine() { canvas.reset(); }

        void run() {

            SDL_Event event;
            bool kill{false}, pause{true};
            uint32_t beginCount{0}, countDelta{7};
            double ts{0.0f};

            while (!kill) {
                beginCount = SDL_GetTicks();
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_EVENT_USER: {

                            switch (static_cast<EventCE>(event.user.code)) {
                                case EventCE::FLOW_PAUSE: {
                                    pause = true;
                                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Paused Receive");
                                } break;
                                case EventCE::FLOW_RESUME: {
                                    pause = false;
                                    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Resume Receive");
                                } break;
                                case EventCE::FLOW_STOP: {
                                    SDL_Event l_eventQuit;
                                    l_eventQuit.type = SDL_EVENT_QUIT;
                                    if (!SDL_PushEvent(&l_eventQuit)) {
                                        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                                                     "Critical SDL_QUIT PushEvent fail: %s", SDL_GetError());
                                    }
                                } break;
                                case EventCE::TOGGLE_FULL_SCREEN:
                                    canvas->toggleFullScreen();
                                    break;
                                default:
                                    break;
                            }
                        }

                        break;
                        case SDL_EVENT_QUIT:
                            kill = true;
                            break;
                        case SDL_EVENT_WINDOW_RESIZED: {
                            canvas->reshape(event.window.data1, event.window.data2);
                        } break;
                        default:
                            break;
                    }

                    for (auto it = stack.end(); it != stack.begin();) {
                        if ((*--it)->onEvent(event) == false)
                            break;
                    }
                }

                ts = (double)countDelta / 1000.0f;
                if (!pause) { // update game
                    for (auto it = stack.begin(); it != stack.end(); it++)
                        (*it)->onUpdate(ts);

                    canvas->before();
                    for (auto it = stack.begin(); it != stack.end(); it++)
                        (*it)->onRender();

                    canvas->after();
                }

                if (timerFPS.stepCount() == true) { // count FPS each second
                    fps = timerFPS.getCountStep();
                    sendChimeraEvent(EventCE::NEW_FPS, (void*)&fps, nullptr);
                }

                countDelta = SDL_GetTicks() - beginCount; // frame count limit
                if (countDelta < miniumCountDelta) {
                    SDL_Delay(miniumCountDelta - countDelta);
                    countDelta = miniumCountDelta;
                }
            }
        }

        inline StateStack& getStack() { return stack; }
    };
} // namespace ce
