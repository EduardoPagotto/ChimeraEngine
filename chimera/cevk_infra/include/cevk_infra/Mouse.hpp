#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <map>

namespace ce {

    /// @brief Mouse Interface
    /// @author <a href="mailto:edupagotto@gmail.com.com">Eduardo Pagotto</a>
    /// @since 20130925
    /// @date 20260907
    class Mouse {

      public:
        Mouse() noexcept { SDL_LogDebug(SDL_LOG_CATEGORY_INPUT, "Mouse init ok"); };
        virtual ~Mouse() noexcept = default;

        [[clang::noinline]] bool getButtonState(const uint8_t& indice) noexcept {
            if (this->buttonState.contains(indice)) {
                return buttonState[indice];
            }

            return false;
        }

        glm::ivec2 getMove() const noexcept { return pos; }

        [[clang::noinline]] glm::ivec2 getMoveRel() noexcept {
            if (flag1 != flag2) {
                flag1 = flag2;
                return rel;
            }
            return glm::ivec2(0);
        }

        [[clang::noinline]] bool getEvent(const SDL_Event& event) noexcept {
            switch (event.type) {
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    this->updateBt(event.button);
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    this->updateMv(event.motion);
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    this->updateWl(event.wheel);
                    break;
                default:
                    return false;
            }

            return true;
        }

        void updateBt(const SDL_MouseButtonEvent& bt) noexcept { this->buttonState[bt.button] = bt.down; }

        void updateWl(const SDL_MouseWheelEvent& mwe) noexcept { this->wheel = glm::ivec2(mwe.x, mwe.y); }

        [[clang::noinline]] void updateMv(const SDL_MouseMotionEvent& mv) noexcept {
            this->pos = glm::ivec2(mv.x, mv.y);
            this->rel = glm::ivec2(mv.xrel, mv.yrel);
            flag1++;
        }

      private:
        std::map<uint8_t, bool> buttonState;
        glm::ivec2 pos{glm::ivec2(0)};
        glm::ivec2 rel{glm::ivec2(0)};
        glm::ivec2 wheel{glm::ivec2(0)};
        uint32_t flag1{0};
        uint32_t flag2{0};
    };
} // namespace ce
